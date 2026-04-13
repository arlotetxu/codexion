/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:36:35 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/13 15:36:36 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

void	*ft_add_to_pq(t_coder *my_coder)
{
	// if (my_coder->left != NULL)
	ft_pq_push(my_coder->left->pq, my_coder);	
	if (my_coder->right && my_coder->right != NULL)
		ft_pq_push(my_coder->right->pq, my_coder);
	// printf("Coder %i añadido a sus dongles.\n", my_coder->id);
	return (NULL);
}

void	ft_take_left_d(t_coder *my_coder)
{
	t_dongle	*left_d;
	int			i_am_first;
	
	left_d = my_coder->left;
	pthread_mutex_lock(&left_d->m_dongle);
	while (1)
	{
		i_am_first = 0;
		pthread_mutex_lock(&left_d->pq->m_pq);
		if (left_d->pq->size > 0 && left_d->pq->heap[0].id == my_coder->id)
			i_am_first = 1;
		else
			i_am_first = 0;
		pthread_mutex_unlock(&left_d->pq->m_pq);
		if(i_am_first && left_d->status == 0 && left_d->end_cool <= ft_get_time_ms())
			break ;
		pthread_cond_wait(&left_d->cond, &left_d->m_dongle);
	}
	left_d->status = 1;
	ft_print_take_dongle(my_coder->id, my_coder->gen);
	pthread_mutex_unlock(&left_d->m_dongle);
}

void	ft_take_right_d(t_coder *my_coder)
{
	t_dongle	*right_d;
	int			i_am_first;
	
	right_d = my_coder->right;
	pthread_mutex_lock(&right_d->m_dongle);
	while (1)
	{
		i_am_first = 0;
		pthread_mutex_lock(&right_d->pq->m_pq);
		if (right_d->pq->size > 0 && right_d->pq->heap[0].id == my_coder->id)
			i_am_first = 1;
		pthread_mutex_unlock(&right_d->pq->m_pq);
		if(i_am_first && right_d->status == 0 && right_d->end_cool <= ft_get_time_ms())
			break ;
		pthread_cond_wait(&right_d->cond, &right_d->m_dongle);
	}
	right_d->status = 1;
	ft_print_take_dongle(my_coder->id, my_coder->gen);
	pthread_mutex_unlock(&right_d->m_dongle);
}

void	ft_take_dongles(t_coder *my_coder)
{
	if (my_coder->id % 2 == 0)
	{
		ft_take_left_d(my_coder);
		ft_take_right_d(my_coder);
	}
	else
	{
		ft_take_right_d(my_coder);
		ft_take_left_d(my_coder);
	}
}

void	ft_release_dongles(t_coder *my_coder)
{
	ft_pq_pop(my_coder->left->pq);	
	ft_pq_pop(my_coder->right->pq);
	
	pthread_mutex_lock(&my_coder->left->m_dongle);
	my_coder->left->status = 0;
	pthread_cond_broadcast(&my_coder->left->cond);
	pthread_mutex_unlock(&my_coder->left->m_dongle);
	
	pthread_mutex_lock(&my_coder->right->m_dongle);
	my_coder->right->status = 0;
	pthread_cond_broadcast(&my_coder->right->cond);
	pthread_mutex_unlock(&my_coder->right->m_dongle);
}