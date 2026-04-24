/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:36:35 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/24 13:44:59 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"

void	ft_sorted_mutex_lock(t_coder *m)
{
	if (m->left < m->right)
	{
		pthread_mutex_lock(&m->left->m_dongle);
		pthread_mutex_lock(&m->right->m_dongle);
	}
	else
	{
		pthread_mutex_lock(&m->right->m_dongle);
		pthread_mutex_lock(&m->left->m_dongle);
	}
}

void	ft_sorted_mutex_unlock(t_coder *m)
{
	if (m->left < m->right)
	{
		pthread_mutex_unlock(&m->right->m_dongle);
		pthread_mutex_unlock(&m->left->m_dongle);
	}
	else
	{
		pthread_mutex_unlock(&m->left->m_dongle);
		pthread_mutex_unlock(&m->right->m_dongle);
	}
}

int	ft_can_take_both(t_coder *m)
{
	t_dongle	*first;
	t_dongle	*second;

	ft_sorted_mutex_lock(m);
	first = m->right;
	second = m->left;
	if (m->id % 2 == 0)
	{
		first = m->left;
		second = m->right;
	}
	if (second == NULL)
		return (0);
	if (first->pq->size > 0 && second->pq->size > 0
		&& first->pq->heap[0]->id == m->id
		&& first->status == 0 && second->status == 0
		&& ft_get_time_ms() >= first->end_cool
		&& ft_get_time_ms() >= second->end_cool)
		return (1);
	ft_sorted_mutex_unlock(m);
	return (0);
}

void	ft_take_dongles(t_coder *m)
{
	m->left->status = 1;
	ft_print_take_dongle(m->id, m->gen);
	m->right->status = 1;
	ft_print_take_dongle(m->id, m->gen);
}

void	ft_release_dongles(t_coder *m)
{
	long	d_cool;

	d_cool = m->gen->p->tt_cooldown;
	m->left->status = 0;
	m->left->end_cool = ft_get_time_ms() + d_cool;
	ft_pq_pop(m->left->pq, m->id);
	m->right->status = 0;
	m->right->end_cool = ft_get_time_ms() + d_cool;
	ft_pq_pop(m->right->pq, m->id);
	ft_sorted_mutex_unlock(m);
}
