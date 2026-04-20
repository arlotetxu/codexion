/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:36:35 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/20 17:53:09 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

/*
	How EDF Works for a Dongle
Just like FIFO, a coder first tries to place itself in the first_in_queue or
second_in_queue slot of the desired dongle.
If the dongle is on cooldown, the coder waits.
Once the dongle is available (not on cooldown), it doesn't automatically go to
first_in_queue. Instead, the dongle compares the last_compile timestamps of the
coders in first_in_queue and second_in_queue (if both are present).
The coder with the earliest last_compile timestamp (meaning they compiled
longest ago and are closest to burnout) is considered the "highest priority"
and gets the dongle.
If the coder currently trying to acquire the dongle is not the highest priority,
it waits and polls again, allowing a higher-priority coder to potentially take
the dongle first.
 */

void	*ft_add_to_pq(t_coder *m)
{
	ft_pq_push(m->left->pq, m);
	if (m->right != NULL)
		ft_pq_push(m->right->pq, m);
	return (NULL);
}

static void	ft_mutex(t_coder *m)
{
	if (m->id % 2 == 0)
	{
		pthread_mutex_lock(&m->left->m_status);
		pthread_mutex_lock(&m->right->m_status);
	}
	else
	{
		pthread_mutex_lock(&m->right->m_status);
		pthread_mutex_lock(&m->left->m_status);
	}
}

int	ft_can_take(t_coder *m)
{
	// pthread_mutex_lock(&m->gen->m_gen);
	ft_mutex(m);
	if (m->left->pq->size > 0 && m->right->pq->size > 0
		&& m->left->pq->heap[0].id == m->id
		&& m->left->status == 0 && m->right->status == 0
		&& ft_get_time_ms() >= m->left->end_cool
		&& ft_get_time_ms() >= m->right->end_cool)
		return (1);
	if (m->id % 2 == 0)
	{
		pthread_mutex_unlock(&m->left->m_status);
		pthread_mutex_unlock(&m->right->m_status);
	}
	else
	{
		pthread_mutex_unlock(&m->right->m_status);
		pthread_mutex_unlock(&m->left->m_status);
	}
	// pthread_mutex_unlock(&m->gen->m_gen);
	return (0);
}

void	ft_take_dongles(t_coder *m)
{
	if (m->id % 2 == 0)
	{
		m->left->status = 1;
		pthread_mutex_unlock(&m->left->m_status);
		ft_print_take_dongle(m->id, m->gen);
		m->right->status = 1;
		pthread_mutex_unlock(&m->right->m_status);
		ft_print_take_dongle(m->id, m->gen);
	}
	else
	{
		m->right->status = 1;
		pthread_mutex_unlock(&m->right->m_status);
		ft_print_take_dongle(m->id, m->gen);
		m->left->status = 1;
		pthread_mutex_unlock(&m->left->m_status);
		ft_print_take_dongle(m->id, m->gen);
	}
}

void	ft_release_dongles(t_coder *m)
{
	long	d_cool;

	d_cool = m->gen->p->tt_cooldown;
	pthread_mutex_lock(&m->left->m_status);
	m->left->status = 0;
	m->left->end_cool = ft_get_time_ms() + d_cool;
	ft_pq_pop(m->left->pq, m->id);
	pthread_mutex_unlock(&m->left->m_status);
	pthread_mutex_lock(&m->right->m_status);
	m->right->status = 0;
	m->right->end_cool = ft_get_time_ms() + d_cool;
	ft_pq_pop(m->right->pq, m->id);
	pthread_mutex_unlock(&m->right->m_status);
}
