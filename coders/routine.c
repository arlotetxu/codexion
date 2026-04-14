/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:24:18 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/14 15:46:09 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
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


void	*ft_start_routine(void *arg)
{
	t_coder	*my_coder;

	my_coder = (t_coder *)arg;
	while (1)
	{
		pthread_mutex_lock(&my_coder->gen->m_gen);
		if (my_coder->is_burned || my_coder->num_comp <= 0)
		{
			pthread_mutex_unlock(&my_coder->gen->m_gen);
			break ;
		}
		pthread_mutex_unlock(&my_coder->gen->m_gen);
		ft_add_to_pq(my_coder);
		ft_take_dongles(my_coder);
		ft_start_compile(my_coder);
		ft_release_dongles(my_coder);
		ft_print_debugging(my_coder);
		ft_print_refactoring(my_coder);
	}
	return (NULL);
}
