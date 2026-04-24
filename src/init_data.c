/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 12:37:41 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/24 13:40:47 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"
#include <stdlib.h>

t_dongle	*ft_init_dongles(t_params *p)
{
	int			i;
	t_dongle	*d;

	d = malloc(sizeof(t_dongle) * p->num_coders);
	if (!p || !d)
		return (NULL);
	i = -1;
	while (++i < p->num_coders)
	{
		if (pthread_mutex_init(&d[i].m_dongle, NULL))
			return (NULL);
		d[i].end_cool = 0;
		d[i].status = 0;
		d[i].pq = malloc(sizeof(t_priority_q));
		d[i].pq->heap = malloc(sizeof(t_coder *) * 2);
		if (!d[i].pq || !d[i].pq->heap)
			return (NULL);
		d[i].pq->size = 0;
		d[i].pq->capacity = p->num_coders;
		d[i].pq->is_edf = p->is_edf;
	}
	return (d);
}

t_coder	*ft_init_coders(t_params *p, t_dongle *d)
{
	int		i;
	t_coder	*c;

	c = malloc(sizeof(t_coder) * p->num_coders);
	if (!p || !c)
		return (NULL);
	i = 0;
	while (i < p->num_coders)
	{
		c[i].id = i + 1;
		c[i].st_comp = ft_get_time_ms();
		c[i].prior = ft_get_time_ms() + p->tt_burn;
		c[i].num_comp = p->num_comp_req;
		c[i].left = &d[i];
		c[i].right = NULL;
		if (p->num_coders > 1)
			c[i].right = &d[(i + 1) % p->num_coders];
		if (pthread_mutex_init(&c[i].m_coder, NULL))
			return (NULL);
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
		return (NULL);
	gen->p = p;
	gen->c = c;
	gen->d = d;
	if (pthread_mutex_init(&gen->m_print, NULL))
		return (NULL);
	if (pthread_mutex_init(&gen->m_gen, NULL))
		return (NULL);
	if (pthread_mutex_init(&gen->m_launch, NULL))
		return (NULL);
	if (pthread_cond_init(&gen->w_align, NULL))
		return (NULL);
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
		return (NULL);
	d = ft_init_dongles(p);
	c = ft_init_coders(p, d);
	gen = ft_init_gen(p, c, d);
	if (!d || !c || !gen)
		return (NULL);
	return (gen);
}
