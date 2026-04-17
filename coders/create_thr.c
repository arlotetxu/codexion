/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:18:34 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/17 12:41:27 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>

void	ft_add_initial_heap(t_gen *g)
{
	int	i;

	i = 0;
	while (i < g->p->num_coders)
	{
		ft_pq_initial_push(g->c[i].left->pq, &g->c[i]);
		ft_pq_initial_push(g->c[i].right->pq, &g->c[i]);
		i++;
	}
	i = 1;
	while (i < g->p->num_coders)
	{
		ft_pq_swap(&g->c[i].left->pq->heap[0], &g->c[i].left->pq->heap[1]);
		i++;
	}
}


int	ft_create_threads(t_gen *g)
{
	int	i;
	pthread_t	*th;

	th = malloc(sizeof(pthread_t) * g->p->num_coders);
	if (!th)
		return (7);
	ft_add_initial_heap(g);
	// ft_print_heaps(g);
	// exit(1);
	i = -1;
	while (++i < g->p->num_coders)
	{
		if(pthread_create(th + i, NULL, ft_start_routine, g->c + i) != 0)
			return (7);
		g->launch++;
	}
	i = -1;
	while (++i < g->p->num_coders)
		if (pthread_join(th[i], NULL) != 0)
			return (7);
	//pthread_destroy
	//pthread_mutex_destroy
	free (th);
	return (0);
}

