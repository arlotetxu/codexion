	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:24:18 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/17 15:55:58 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void	ft_wait_coders(t_coder *m)
{
	pthread_mutex_lock(&m->gen->m_launch);
	while (m->gen->launch != 1)
		pthread_cond_wait(&m->gen->w_align, &m->gen->m_launch);
	pthread_mutex_unlock(&m->gen->m_launch);
	return ;
}

void	*ft_start_routine(void *arg)
{
	t_coder	*m;

	m = (t_coder *)arg;
	ft_wait_coders(m);
	while (1)
	{
		pthread_mutex_lock(&m->gen->m_gen);
		if (m->is_burned || m->num_comp <= 0)
			break ;
		pthread_mutex_unlock(&m->gen->m_gen);
		if (ft_can_take_both(m))
		{
			ft_take_dongles(m);
			ft_start_compile(m);
			if (m->num_comp)
				ft_add_to_pq(m);
		}
		usleep(10);
	}
	pthread_mutex_unlock(&m->gen->m_gen);
	return (NULL);
}
