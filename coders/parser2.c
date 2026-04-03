/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:05:36 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/03 17:42:48 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int	ft_num_args(char **all_args)
{
	int	num_args;

	num_args = 0;
	while (all_args[num_args])
		num_args++;
	return (num_args);
}

int	ft_check_args_nums(char **args)
{
	int	i;
	int	len;

	len = ft_count_args(args);
	i = -1;
	while (++i < len - 1)
		if (ft_not_all_digits(args[i]))
			return (2);
	return (0);
}

int	ft_check_arg_int(char **args)
{
	int		i;
	int		len;
	long	nb;

	len = ft_count_args(args);
	i = -1;
	while (++i < len -1)
	{
		nb = ft_atoi(args[i]);
		if (nb == -1)
			return (3);
	}
	return (0);
}

int	ft_check_last_arg(char **args)
{
	int		last_index;
	char	*last_arg;
	int		is_fifo;
	int		is_edx;

	last_index = ft_count_args(args);
	last_index = last_index - 1;
	last_arg = ft_to_lower(args[last_index]);
	is_fifo = ft_strcmp(last_arg, "fifo");
	is_edx = ft_strcmp(last_arg, "edf");
	if (!is_fifo && !is_edx)
	{
		free(last_arg);
		return (4);
	}
	free(last_arg);
	return (0);
}

int	ft_loading_params(char **all_args, t_params *p_params)
{
	// t_params	*p_params;
	// p_params = malloc(sizeof(t_params)); //!malloc OK
	// if (!p_params)
	// 	return (-1);
	p_params->num_coders = ft_atoi(all_args[0]);
	p_params->tt_burn = ft_atoi(all_args[1]);
	p_params->tt_comp = ft_atoi(all_args[2]);
	p_params->tt_deb = ft_atoi(all_args[3]);
	p_params->tt_ref = ft_atoi(all_args[4]);
	p_params->num_comp_req = ft_atoi(all_args[5]);
	p_params->tt_cooldown = ft_atoi(all_args[6]);
	p_params->scheduler = ft_to_lower(all_args[7]); //!malloc sin liberar
	ft_double_free(NULL, all_args);
	return (0);
}
