/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:58:14 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/24 17:02:47 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"
#include <stdlib.h>

/*
RETURN ERROR CODES
1 - Wrong number of entry parameters
2 - There are not numbers in the first seven arguments
3 - Integer limits exceeded
4 - scheduler is not fifo/edf
5 - Issues saving input parameters (memory allocation)
6 - The general struct gen coudln't be created properly
7 - Issues creating/joinning threads
8 - Issues creating/joinning watcher thread.
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
		return (6);
	ret = ft_create_watcher(gen);
	if (ret)
		return (ft_free_gen_struct(gen), ret);
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
	p_param = malloc(sizeof(t_params));
	if (!p_param)
		return (ft_print_error(5), 5);
	ret = ft_start_program(all_args, p_param);
	if (ret)
		return (ft_double_free(p_param, all_args), ret);
	return (0);
}
