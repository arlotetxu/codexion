/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:36:35 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/17 17:07:24 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void	*ft_add_to_pq(t_coder *my_coder)
{
	ft_pq_push(my_coder->left->pq, my_coder);
	if (my_coder->right != NULL)
		ft_pq_push(my_coder->right->pq, my_coder);
	return (NULL);
}

int	ft_can_take(t_coder *m)
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
	// if(m->left->pq->size > 0 && m->right->pq->size > 0 &&
	// 	m->left->pq->heap[0].id == m->id && m->right->pq->heap[0].id == m->id &&
	// 	m->left->status == 0 && m->right->status == 0)
	if(m->left->pq->size > 0 && m->right->pq->size > 0 &&
		m->left->pq->heap[0].id == m->id &&
		m->left->status == 0 && m->right->status == 0 &&
		ft_get_time_ms() >= m->left->end_cool
		&& ft_get_time_ms() >= m->left->end_cool)
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
	// pthread_mutex_unlock(&m->left->m_status);
	// pthread_mutex_unlock(&m->right->m_status);
	return (0);
}

// void	ft_take_dongle(t_coder *my_coder, t_dongle *d)
// {
// 	long	wait_time;

// 	pthread_mutex_lock(&d->m_dongle);
// 	while (1)
// 	{
// 		pthread_mutex_lock(&d->pq->m_pq);
// 		if(d->pq->size > 0 && d->pq->heap[0].id == my_coder->id
// 			&& d->status == 0)
// 		{
// 			pthread_mutex_unlock(&d->pq->m_pq);
// 			wait_time = d->end_cool - ft_get_time_ms();
// 			if (wait_time <= 0)
// 				break;
// 			pthread_mutex_unlock(&d->m_dongle);
// 			ft_sleep_ms(wait_time);
// 			pthread_mutex_lock(&d->m_dongle);
// 			continue;
// 		}
// 		pthread_mutex_unlock(&d->pq->m_pq);
// 		pthread_cond_wait(&d->cond, &d->m_dongle);
// 	}
// 	d->status = 1;
// 	ft_print_take_dongle(my_coder->id, my_coder->gen);
// 	pthread_mutex_unlock(&d->m_dongle);
// }

void	ft_take_dongle(t_coder *m, t_dongle *d)
{
	// long	wait_time;

	// wait_time = d->end_cool - ft_get_time_ms();
	// pthread_mutex_lock(&d->m_status);

	d->status = 1;
	pthread_mutex_unlock(&d->m_status);
	// if (wait_time > 0)
		// ft_sleep_ms(wait_time);

	ft_print_take_dongle(m->id, m->gen);
}

void	ft_take_dongles(t_coder *my_coder)
{
	if (my_coder->id % 2 == 0)
	// if (my_coder->left < my_coder->right)
	{
		ft_take_dongle(my_coder, my_coder->left);
		ft_take_dongle(my_coder, my_coder->right);
	}
	else
	{
		ft_take_dongle(my_coder, my_coder->right);
		ft_take_dongle(my_coder, my_coder->left);
	}
}

// void	ft_take_dongles(t_coder *my_coder)
// {
// 	ft_take_dongle(my_coder, my_coder->left);
// 	ft_take_dongle(my_coder, my_coder->right);
// }

void	ft_release_dongles(t_coder *my_coder)
{
	long	d_cool;

	d_cool = my_coder->gen->p->tt_cooldown;

	// pthread_mutex_lock(&my_coder->left->m_dongle);
	pthread_mutex_lock(&my_coder->left->m_status);
	my_coder->left->status = 0;

	my_coder->left->end_cool = ft_get_time_ms() + d_cool;
	ft_pq_pop(my_coder->left->pq, my_coder->id);
	pthread_mutex_unlock(&my_coder->left->m_status);
	// ft_pq_swap(my_coder->left->pq->heap + 0, my_coder->left->pq->heap + 1);
	// pthread_cond_broadcast(&my_coder->left->cond);

	// pthread_mutex_lock(&my_coder->right->m_dongle);
	pthread_mutex_lock(&my_coder->right->m_status);
	my_coder->right->status = 0;

	my_coder->right->end_cool = ft_get_time_ms() + d_cool;
	ft_pq_pop(my_coder->right->pq, my_coder->id);
	pthread_mutex_unlock(&my_coder->right->m_status);
	// ft_pq_swap(my_coder->right->pq->heap + 0, my_coder->right->pq->heap + 1);
	// pthread_cond_broadcast(&my_coder->right->cond);
}
