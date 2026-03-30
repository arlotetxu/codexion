/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:00:50 by joflorid          #+#    #+#             */
/*   Updated: 2026/03/30 17:03:19 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"
#include<stdio.h>
#include<stdlib.h>

int	ft_args_len(int argc, char **argv)
{
	int	i;
	int	j;
	int	len;

	len = 0;
	i = 0;
	while (argv[++i])
	{
		j = -1;
		while (argv[i][++j])
			len++;
	}
	return (len + argc - 1);
}

char	*ft_join(int argc, char **argv)
{
	int	i;
	int	j;
	int	k;
	char	*all_argv;

	k = 0;
	i = 0;
	all_argv = malloc((sizeof(char) * ft_args_len(argc, argv)) + 1);
	if (!all_argv)
		return (NULL);
	while(argv[++i])
	{
		j = -1;
		while(argv[i][++j])
		{
			all_argv[k] = argv[i][j];
			k++;
		}
		if (i < argc - 1)
			all_argv[k++] = ' ';
	}
	all_argv[k] = '\0';
	return (all_argv);
}
