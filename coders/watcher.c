/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 13:11:28 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/22 17:24:30 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	ft_check_burnout(t_gen *g)
{
	int	i;

	i = -1;
	while (++i < g->p->num_coders)
	{

		pthread_mutex_lock(&g->c[i].m_coder);
		if ((ft_get_time_ms() - g->c[i].st_comp) > g->p->tt_burn)
		{
			pthread_mutex_unlock(&g->c[i].m_coder);
			pthread_mutex_lock(&g->m_gen);
			g->stop_sim = 1;
			pthread_mutex_unlock(&g->m_gen);
			// g->c[i].is_burned = 1;

			return (1);
		}
		pthread_mutex_unlock(&g->c[i].m_coder);

		// pthread_mutex_unlock(&g->m_gen);
		usleep(10);
	}
	// pthread_mutex_unlock(&g->m_gen);
	return (0);
}

void	*ft_w_routine(void *arg)
{
	t_gen	*g;

	g = (t_gen *)arg;

	while (1)
	{
		if(ft_check_burnout(g) == 1)
			break ;
		pthread_mutex_lock(&g->m_gen);
		if (g->pending_comp == 0)
		{
			pthread_mutex_unlock(&g->m_gen);
			break ;
		}
		pthread_mutex_unlock(&g->m_gen);
	}
	return (NULL);
}

int	ft_create_watcher(t_gen *g)
{
	if (pthread_create(&g->watcher, NULL, ft_w_routine, g))
		return (8);
	return (0);
}
