/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:58:14 by joflorid          #+#    #+#             */
/*   Updated: 2026/03/31 12:51:25 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"codexion.h"
#include<stdio.h>
#include<stdlib.h>


int main(int argc, char **argv)
{
	char	*arg_join;
	char	**all_args;
	int		i;

	arg_join = ft_arg_join(argc, argv);
	all_args = ft_arg_split(arg_join, 32);

	i = -1;
	while (all_args[++i])
		printf("Cadena[%d]: %s\n", i, all_args[i]);

	free(arg_join);
	ft_double_free(all_args);

	return (0);
}
