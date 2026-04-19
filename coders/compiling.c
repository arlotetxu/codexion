/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compiling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:30:53 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/17 17:44:28 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

void	ft_start_compile(t_coder *m)
{
	pthread_mutex_lock(&m->m_coder);
	m->st_comp = ft_get_time_ms();
	m->prior = ft_get_time_ms() + m->gen->p->tt_burn;
	m->num_comp--;
	pthread_mutex_unlock(&m->m_coder);
	ft_print_compiling(m);
	ft_release_dongles(m);
	ft_print_debugging(m);
	ft_print_refactoring(m);
}
