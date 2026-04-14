/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 12:37:41 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/14 15:59:57 by joflorid         ###   ########.fr       */
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
		pthread_cond_init(&d[i].cond, NULL);
		d[i].end_cool = 0;
		d[i].status = 0;
		d[i].pq = malloc(sizeof(t_priority_q)); //!malloc sin liberar
		if (!d[i].pq)
			return (NULL); //!Liberar
		d[i].pq->heap = malloc(sizeof(t_coder) * p->num_coders); //!malloc sin liberar
		if (!d[i].pq->heap)
			return (NULL); //!Liberar
		d[i].pq->size = 0;
		d[i].pq->capacity = p->num_coders;
		d[i].pq->is_edf = p->is_edf;
		pthread_mutex_init(&d[i].pq->m_pq, NULL);
	}
	return (d);
}

t_coder	*ft_init_coders(t_params *p, t_dongle *d)
{
	int		i;
	t_coder	*c;
	struct	timeval tv;

	gettimeofday(&tv, NULL);
	c = malloc(sizeof(t_coder) * p->num_coders ); //!malloc sin liberar
	if (!p || !c)
		return (NULL); //!Liberar?
	i = 0;
	while (i < p->num_coders)
	{
		c[i].id = i + 1;
		c[i].st_comp = tv.tv_sec * 1000;
		c[i].st_deb = 0;
		c[i].st_ref = 0;
		c[i].prior = (tv.tv_sec * 1000) + p->tt_burn;
		c[i].num_comp = p->num_comp_req;
		c[i].is_burned = 0;
		c[i].left = &d[i];
		c[i].right = NULL;
		if (p->num_coders > 1)
			c[i].right = &d[(i + 1) % p->num_coders];
		i++;
	}
	return (c);
}

t_gen	*ft_init_gen(t_params *p, t_coder *c, t_dongle *d)
{
	t_gen	*gen;
	int		i;
	struct	timeval tv;

	gettimeofday(&tv, NULL);

	gen = malloc(sizeof(t_gen));
	if (!p || !c || !d || !gen)
		return (NULL); //!Liberar?
	gen->p = p;
	gen->c = c;
	gen->d = d;
	pthread_mutex_init(&gen->end_sim, NULL);
	pthread_mutex_init(&gen->m_print, NULL);
	gen->stop_sim = 0;
	gen->init_time = ft_get_time_ms();
	i = -1;
	while(++i < p->num_coders)
		c[i].gen = gen;
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
	pthread_mutex_init(&gen->m_gen, NULL);
	if (!d || !c || !gen)
		return (NULL);
	return (gen);
}
