/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:36:35 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/14 17:48:03 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <unistd.h>

void	*ft_add_to_pq(t_coder *my_coder)
{
	ft_pq_push(my_coder->left->pq, my_coder);
	if (my_coder->right && my_coder->right != NULL)
		ft_pq_push(my_coder->right->pq, my_coder);
	return (NULL);
}

void	ft_take_dongle(t_coder *my_coder, t_dongle *d)
{
	long	wait_time;

	pthread_mutex_lock(&d->m_dongle);
	while (1)
	{
		pthread_mutex_lock(&d->pq->m_pq);
		if(d->pq->size > 0 && d->pq->heap[0].id == my_coder->id
			&& d->status == 0)
		{
			pthread_mutex_unlock(&d->pq->m_pq);
			wait_time = d->end_cool - ft_get_time_ms();
			if (wait_time <= 0)
				break;
			pthread_mutex_unlock(&d->m_dongle);
			usleep(wait_time * 1000);
			pthread_mutex_lock(&d->m_dongle);
			continue;
		}
		pthread_mutex_unlock(&d->pq->m_pq);
		pthread_cond_wait(&d->cond, &d->m_dongle);
	}
	d->status = 1;
	ft_print_take_dongle(my_coder->id, my_coder->gen);
	pthread_mutex_unlock(&d->m_dongle);
}

void	ft_take_dongles(t_coder *my_coder)
{
	if (my_coder->id % 2 == 0)
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

void	ft_release_dongles(t_coder *my_coder)
{
	long	d_cool;

	d_cool = my_coder->gen->p->tt_cooldown;
	ft_pq_pop(my_coder->left->pq, my_coder->id);
	ft_pq_pop(my_coder->right->pq, my_coder->id);

	pthread_mutex_lock(&my_coder->left->m_dongle);
	my_coder->left->status = 0;
	my_coder->left->end_cool = ft_get_time_ms() + d_cool;
	pthread_cond_broadcast(&my_coder->left->cond);
	pthread_mutex_unlock(&my_coder->left->m_dongle);

	pthread_mutex_lock(&my_coder->right->m_dongle);
	my_coder->right->status = 0;
	my_coder->right->end_cool = ft_get_time_ms() + d_cool;
	pthread_cond_broadcast(&my_coder->right->cond);
	pthread_mutex_unlock(&my_coder->right->m_dongle);
}
