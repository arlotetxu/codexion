/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:24:18 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/24 17:03:23 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"
#include <unistd.h>

void	ft_wait_coders(t_coder *m)
{
	pthread_mutex_lock(&m->gen->m_launch);
	while (m->gen->launch != 1)
		pthread_cond_wait(&m->gen->w_align, &m->gen->m_launch);
	pthread_mutex_unlock(&m->gen->m_launch);
	return ;
}

int	ft_exit_routine(t_coder *m)
{
	pthread_mutex_lock(&m->gen->m_gen);
	if (m->gen->stop_sim)
	{
		pthread_mutex_unlock(&m->gen->m_gen);
		return (1);
	}
	pthread_mutex_unlock(&m->gen->m_gen);
	pthread_mutex_lock(&m->m_coder);
	if (m->num_comp <= 0)
	{
		pthread_mutex_unlock(&m->m_coder);
		return (1);
	}
	pthread_mutex_unlock(&m->m_coder);
	return (0);
}

void	*ft_start_routine(void *arg)
{
	t_coder	*m;

	m = (t_coder *)arg;
	if (m->gen->p->num_coders == 1)
	{
		ft_one_coder(m->gen);
		return (NULL);
	}
	ft_wait_coders(m);
	if (m->id % 2 != 0)
		usleep(2000);
	while (1)
	{
		if (ft_exit_routine(m))
			break ;
		if (ft_can_take_both(m))
		{
			ft_take_dongles(m);
			ft_start_compile(m);
			if (m->num_comp)
				ft_add_to_pq(m);
		}
		usleep(100);
	}
	return (NULL);
}
