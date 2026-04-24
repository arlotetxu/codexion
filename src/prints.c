/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 13:07:40 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/24 17:03:19 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"
#include <stdio.h>

int	ft_print_error(int err_nb)
{
	if (err_nb == 1)
		printf("[ERROR]-Wrong number of arguments.\n");
	else if (err_nb == 2)
		printf("[ERROR]-First 7 arguments must be numeric and positives "
			"numbers.\n");
	else if (err_nb == 3)
		printf("[ERROR]-Arguments must be within int limits.\n");
	else if (err_nb == 4)
		printf("[ERROR]-Last argument not valid (valid options: fifo/edf).\n");
	else if (err_nb == 5)
		printf("[ERROR]-Input parameters couldn't be saved.\n");
	return (err_nb);
}

void	ft_print_take_dongle(int id, t_gen *g)
{
	long			time;

	pthread_mutex_lock(&g->m_gen);
	if (g->stop_sim)
	{
		pthread_mutex_unlock(&g->m_gen);
		return ;
	}
	pthread_mutex_unlock(&g->m_gen);
	time = (ft_get_time_ms() - g->init_time);
	pthread_mutex_lock(&g->m_print);
	printf("%li %i has taken a dongle\n", time, id);
	pthread_mutex_unlock(&g->m_print);
}

void	ft_print_compiling(t_coder *m)
{
	long			time;
	long			sleep;
	long			init_time;

	pthread_mutex_lock(&m->gen->m_gen);
	if (m->gen->stop_sim)
	{
		pthread_mutex_unlock(&m->gen->m_gen);
		return ;
	}
	pthread_mutex_unlock(&m->gen->m_gen);
	init_time = m->gen->init_time;
	time = (ft_get_time_ms() - init_time);
	sleep = ft_get_time_ms() + m->gen->p->tt_comp;
	pthread_mutex_lock(&m->gen->m_print);
	printf("\e[0;32m%li %i is compiling\n\e[0m", time, m->id);
	pthread_mutex_unlock(&m->gen->m_print);
	while (ft_get_time_ms() < sleep)
	{
	}
}

void	ft_print_debugging(t_coder *m)
{
	long			time;
	long			sleep;
	long			init_time;

	pthread_mutex_lock(&m->gen->m_gen);
	if (m->gen->stop_sim)
	{
		pthread_mutex_unlock(&m->gen->m_gen);
		return ;
	}
	pthread_mutex_unlock(&m->gen->m_gen);
	init_time = m->gen->init_time;
	time = (ft_get_time_ms() - init_time);
	sleep = ft_get_time_ms() + m->gen->p->tt_deb;
	pthread_mutex_lock(&m->gen->m_print);
	printf("\e[0;36m%li %i is debugging\n\e[0m", time, m->id);
	pthread_mutex_unlock(&m->gen->m_print);
	while (ft_get_time_ms() < sleep)
	{
	}
}

void	ft_print_refactoring(t_coder *m)
{
	long			time;
	long			sleep;
	long			init_time;

	pthread_mutex_lock(&m->gen->m_gen);
	if (m->gen->stop_sim)
	{
		pthread_mutex_unlock(&m->gen->m_gen);
		return ;
	}
	pthread_mutex_unlock(&m->gen->m_gen);
	init_time = m->gen->init_time;
	time = (ft_get_time_ms() - init_time);
	sleep = ft_get_time_ms() + m->gen->p->tt_ref;
	pthread_mutex_lock(&m->gen->m_print);
	printf("\e[0;33m%li %i is refactoring\n\e[0m", time, m->id);
	pthread_mutex_unlock(&m->gen->m_print);
	while (ft_get_time_ms() < sleep)
	{
	}
}
