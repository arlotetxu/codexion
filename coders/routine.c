/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:24:18 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/17 15:55:58 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
/*
	Si piensas en el ciclo de vida de tu hilo dentro del `while (1)`, el flujo correcto es este:

1.  **Tengo hambre (quiero compilar):** Me apunto en la lista de espera de mis
	dos dongles (Añado a las dos `pq`).
2.  **Espero mi turno:** Intento coger los dongles. Si no puedo, me duermo con
	`pthread_cond_wait` hasta que sea mi turno y estén libres.
3.  **Compilo:** Uso los dongles (hago un `usleep` simulando el tiempo de compilación).
4.  **Termino y suelto:** Marco los dongles como libres (`status = 0`),
	actualizo su `end_cool`, **ME BORRO de las colas de prioridad** (porque ya
	he terminado mi turno), y aviso a los demás (`pthread_cond_broadcast` o
	`signal`) de que los dongles están libres.
5.  **Depuro y Refactorizo:** Hago mis otras tareas (dormir/pensar).
6.  **Vuelvo al paso 1
 */
void	ft_wait_coders(t_coder *m)
{
	while (1)
	{
		pthread_mutex_lock(&m->gen->m_launch);
		if (m->gen->launch == m->gen->p->num_coders)
			break ;
		pthread_mutex_unlock(&m->gen->m_launch);
	}
	pthread_mutex_unlock(&m->gen->m_launch);
	return ;
}

void	*ft_start_routine(void *arg)
{
	t_coder	*my_coder;

	my_coder = (t_coder *)arg;
	ft_wait_coders(my_coder);
	// ft_add_to_pq(my_coder);;
	if (my_coder->id % 2 == 0)
			usleep(20);
	while (1)
	{
		pthread_mutex_lock(&my_coder->gen->m_gen);
		if (my_coder->is_burned || my_coder->num_comp <= 0)
		{
			pthread_mutex_unlock(&my_coder->gen->m_gen);
			break ;
		}
		pthread_mutex_unlock(&my_coder->gen->m_gen);
		if (ft_can_take(my_coder))
		{
			// pthread_mutex_unlock(&my_coder->left->m_status);
			// pthread_mutex_unlock(&my_coder->right->m_status);
			ft_take_dongles(my_coder);
			ft_start_compile(my_coder);
			ft_release_dongles(my_coder);
			ft_add_to_pq(my_coder);
		}
	}
	// ft_print_heaps(my_coder->gen);
	return (NULL);
}
