/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:05:36 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/01 17:20:53 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"codexion.h"
#include<limits.h>
#include<stdio.h>

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

	// len = 0;
	// while (args[len])
	// 	len++;
	len = ft_count_args(args);
	i = -1;
	while (++i < len - 1)
		if (ft_not_all_digits(args[i]))
			return (2);
	return (0);
}

int	ft_check_arg_long(char **args)
{
	int		i;
	int		len;
	long long	nb;

	// len = 0;
	// while (args[len])
	// 	len++;
	len = ft_count_args(args);
	i = -1;
	while (++i < len -1)
	{
		nb = ft_atol(args[i]);
		if (nb == -1)
			return (3);
	}
	return (0);
}

int	ft_check_last_arg(char **args)
{
	int		last_index;
	char	*last_arg;

	last_index = ft_count_args(args);
	last_index = last_index - 1;
	last_arg = ft_to_lower(args[last_index]);
	// printf("Last arg: %s\n", last_arg);
	return (0);
}
