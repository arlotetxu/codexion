/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:58:14 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/22 17:04:43 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
PARSEO:
- Numero de argumentos correctos [X] - 1
- Todos los argumentos excepto ultimo son numeros [X] - 2
- Todos los argumentos excepto ultimo son positivos [X] - 2
- Todos los argumentos numericos no exceden un long [X] - 3
- Ultimo argumento solo puede ser fifo / edf (convertir minus) [X] - 4
- Añadir parametros de entrada a la estructura general t_params


CODIGO ERRORES
1 - Numero incorrecto de argumentos
2 - Primeros 7 argumentos no son numericos
3 - Algun argumento excede los limites de int
4 - La logica elegida no es ni fifo ni edf
5 - No se han podido guardar los parametros de entrada (memory allocation)
6 - No se ha podido generar la estructura ppal gen
7 - Error al crear o unir hilos


TO DO

- [X] Crear marcas de tiempo en impresion
- [X] Controlar enfriamiento de dongles
- [X] Actualizar los tiempos en la rutina de los coders
- [X] Evaluar si las variables st_deb y st_ref en t_coder son necesarias
- [] Crear hilo watcher con su rutina
- [] Proteger mutex_init en init_data.c
- [] Desarrollar escenario para 1 solo coder
- [] Destruir hilos
- [] Destruir mutex
- [] README

*/

int	ft_start_program(char **all_args, t_params *p_param)
{
	int		ret;
	t_gen	*gen;

	ret = 0;
	if (ft_num_args(all_args) != 8)
		return (ft_print_error(1), 1);
	ret = ft_start_parsing(all_args, p_param);
	if (ret)
		return (ret);
	gen = ft_start_init_data(p_param);
	if (!gen)
		return (6); //!Liberar?
	// Crear watcher
	ret = ft_create_watcher(gen);
	if (ret)
		return (ft_free_gen_struct(gen), ret);
	// Crear hilos
	ret = ft_create_threads(gen);
	if (ret)
		return (ft_free_gen_struct(gen), ret);
	ret = pthread_join(gen->watcher, NULL);
	if (ret != 0)
		return (ft_free_gen_struct(gen), 8);
	ft_free_gen_struct(gen);
	return (0);
}

int	main(int argc, char **argv)
{
	char		*arg_join;
	char		**all_args;
	int			ret;
	t_params	*p_param;

	arg_join = ft_arg_join(argc, argv);
	all_args = ft_arg_split(arg_join, 32);
	free(arg_join);
	p_param = malloc(sizeof(t_params)); //!malloc OK
	if (!p_param)
		return (ft_print_error(5), 5);
	ret = ft_start_program(all_args, p_param);
	if (ret)
		return (ft_double_free(p_param, all_args), ret);
	printf("Todo OK hasta el momento!!\n");
	free(p_param);
	return (0);
}
