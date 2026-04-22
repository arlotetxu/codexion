/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 12:37:41 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/22 17:23:28 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

t_dongle	*ft_init_dongles(t_params *p)
{
	int			i;
	t_dongle	*d;

	d = malloc(sizeof(t_dongle) * p->num_coders); //!malloc sin liberar
	if (!p || !d)
		return (NULL); //!Liberar?
	i = -1;
	while (++i < p->num_coders)
	{
		pthread_mutex_init(&d[i].m_dongle, NULL);
		// pthread_mutex_init(&d[i].m_status, NULL);
		pthread_cond_init(&d->w_coold, NULL);
		d[i].end_cool = 0;
		d[i].status = 0;
		d[i].pq = malloc(sizeof(t_priority_q)); //!malloc sin liberar
		d[i].pq->heap = malloc(sizeof(t_coder) * 2); //!malloc sin liberar
		if (!d[i].pq || !d[i].pq->heap)
			return (NULL); //!Liberar
		d[i].pq->size = 0;
		d[i].pq->capacity = p->num_coders;
		d[i].pq->is_edf = p->is_edf;
		// pthread_mutex_init(&d[i].pq->m_pq, NULL);
	}
	return (d);
}

t_coder	*ft_init_coders(t_params *p, t_dongle *d)
{
	int		i;
	t_coder	*c;

	c = malloc(sizeof(t_coder) * p->num_coders); //!malloc sin liberar
	if (!p || !c)
		return (NULL); //!Liberar?
	i = 0;
	while (i < p->num_coders)
	{
		c[i].id = i + 1;
		c[i].st_comp = ft_get_time_ms();
		c[i].prior = ft_get_time_ms() + p->tt_burn;
		c[i].num_comp = p->num_comp_req;
		c[i].is_burned = 0;
		c[i].left = &d[i];
		c[i].right = NULL;
		if (p->num_coders > 1)
			c[i].right = &d[(i + 1) % p->num_coders];
		pthread_mutex_init(&c[i].m_coder, NULL);
		// pthread_mutex_init(&c->m_st_comp, NULL);
		i++;
	}
	return (c);
}

t_gen	*ft_init_gen(t_params *p, t_coder *c, t_dongle *d)
{
	t_gen	*gen;
	int		i;

	gen = malloc(sizeof(t_gen));
	if (!p || !c || !d || !gen)
		return (NULL); //!Liberar?
	gen->p = p;
	gen->c = c;
	gen->d = d;
	// pthread_mutex_init(&gen->m_stop_sim, NULL);
	pthread_mutex_init(&gen->m_print, NULL);
	pthread_mutex_init(&gen->m_gen, NULL);
	pthread_mutex_init(&gen->m_launch, NULL);
	pthread_cond_init(&gen->w_align, NULL);
	gen->stop_sim = 0;
	gen->init_time = ft_get_time_ms();
	gen->launch = 0;
	i = -1;
	while (++i < p->num_coders)
		c[i].gen = gen;
	gen->pending_comp = gen->p->num_coders * gen->p->num_comp_req;
	return (gen);
}

t_gen	*ft_start_init_data(t_params *p)
{
	t_dongle	*d;
	t_coder		*c;
	t_gen		*gen;

	if (!p)
		return (NULL); //!Liberar?
	d = ft_init_dongles(p);
	c = ft_init_coders(p, d);
	gen = ft_init_gen(p, c, d);
	if (!d || !c || !gen)
		return (NULL);
	return (gen);
}
