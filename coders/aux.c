/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:16:09 by joflorid          #+#    #+#             */
/*   Updated: 2026/03/31 12:53:12 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"codexion.h"
#include<stdlib.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_count_words(char *str, char sep)
{
	int	i;
	int	flag;
	int	count;

	count = 0;
	flag = 0;
	i = 0;
	while (str[i] == sep)
		i++;
	while (str[i++])
	{
		while (str[i++] != sep)
			flag = 1;
		if (flag == 1)
		{
			count++;
			flag = 0;
		}
	}
	if (flag == 1)
		count++;
	return (count);
}

void	ft_double_free(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
}
