/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 13:07:40 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/03 11:15:27 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <_time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int	ft_print_error(int err_nb)
{
	if (err_nb == 1)
		printf("[ERROR]-Wrong number of arguments.\n");
	else if (err_nb == 2)
		printf("[ERROR]-First 7 arguments must be numeric and positives "
			"numbers (without signs).\n");
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
	pthread_mutex_lock(&g->m_print);
	printf("%i has taken a dongle\n", id);
	pthread_mutex_unlock(&g->m_print);
}

void	ft_print_compiling(t_coder *my_coder)
{
	pthread_mutex_lock(&my_coder->gen->m_print);
	printf("\e[0;32m%i is compiling\n\e[0m", my_coder->id);
	pthread_mutex_unlock(&my_coder->gen->m_print);
	usleep(my_coder->gen->p->tt_comp * 1000);
}

void	ft_print_debugging(t_coder *my_coder)
{
	pthread_mutex_lock(&my_coder->gen->m_print);
	printf("\e[0;36m%i is debugging\n\e[0m", my_coder->id);
	pthread_mutex_unlock(&my_coder->gen->m_print);
	usleep(my_coder->gen->p->tt_deb * 1000);
}

void	ft_print_refactoring(t_coder *my_coder)
{
	pthread_mutex_lock(&my_coder->gen->m_print);
	printf("\e[0;33m%i is refactoring\n\e[0m", my_coder->id);
	pthread_mutex_unlock(&my_coder->gen->m_print);
	usleep(my_coder->gen->p->tt_ref * 1000);
}