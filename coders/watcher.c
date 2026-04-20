/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 13:11:28 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/20 17:00:48 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

// int	ft_check_burnout(t_gen *g)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < g->p->num_coders)
// 	{
// 		pthread_mutex_lock(&g->c->m_st_comp);
// 		if ((ft_get_time_ms() - g->c[i].st_comp) > g->p->tt_burn)
// 		{

// 		}
// 		//fuera
// 		usleep(10);
// 	}
// }

// void	*ft_w_routine(void *arg)
// {
// 	t_gen *g;

// 	g = (t_gen *)arg;
// 	while (1)
// 	{
// 		//comprobar burnouts --> Salida con free de estructuras, y destroy de
// 		//	hilos y mutexes
// 		//comprobar que todos los coders han completado las compilaciones -->
// 		//	Salida con free de estructuras
// 	}
// 	return (NULL);
// }

// int	ft_create_watcher(t_gen *g)
// {
// 	pthread_t	*watcher;

// 	watcher = malloc(sizeof(pthread_t));
// 	if (!watcher)
// 		return (8);
// 	if (pthread_create(&watcher, NULL, ft_w_routine, g))
// 		return (8);
// 	if (pthread_join(watcher, NULL))
// 		return (8);
// 	//pthread_destroy
// 	//pthread_mutex_destroy
// }
