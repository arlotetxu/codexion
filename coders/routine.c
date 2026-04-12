/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:24:18 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/10 12:59:40 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void	*ft_add_to_pq(t_coder *my_coder)
{
	if (my_coder->left != NULL)
		ft_pq_push(my_coder->left->pq, my_coder);
	if (my_coder->right != NULL)
		ft_pq_push(my_coder->right->pq, my_coder);
	//Poner en espera del cond de los dongles hasta que el coder sea el primero de la PQ
	printf("Coder %i añadido a sus dongles.\n", my_coder->id);
	return (NULL);
}

void	*ft_start_routine(void *arg)
{
	t_coder	*my_coder;

	my_coder = (t_coder *)arg;
	ft_add_to_pq(my_coder);
	while (1)
	{
		// 1. Comprobar si debemos salir
		// (Falta proteger estas lecturas con mutex general si se modifican desde otro hilo)
		if (my_coder->is_burned || my_coder->num_comp <= 0)
			break ;

		// 2. Intentar coger los dongles (Aquí es donde añadirás a la PQ y esperarás)
		// ft_take_dongles(my_coder);
		
		// BORRAR O COMENTAR ESTO POR AHORA PARA QUE NO FALLE EL BUCLE
		// ft_add_to_pq(my_coder); 

		// 3. Compilar (Compile)
		// printf("Coder %d is compiling\n", my_coder->id);
		// usleep(tt_comp * 1000); 

		// 4. Soltar los dongles
		// ft_release_dongles(my_coder);

		// 5. Depurar (Debug)
		// printf("Coder %d is debugging\n", my_coder->id);
		// usleep(tt_deb * 1000);

		// 6. Refactorizar (Refactor)
		// printf("Coder %d is reflecting\n", my_coder->id);
	}
			return (NULL);
}
