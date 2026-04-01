/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:16:09 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/01 16:38:40 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"codexion.h"
#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

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
	while (str[i])
	{
		if (str[i++] != sep)
			flag = 1;
		else
		{
			if (flag == 1)
			{
				count++;
				flag = 0;
			}
		}
	}
	if (flag == 1)
		count++;
	return (count);
}

void	ft_double_free(char *str, char **str2)
{
	int	i;

	if (str)
		free(str);
	i = -1;
	while (str2[++i])
		free(str2[i]);
	free(str2);
}

int	ft_not_all_digits(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] < 48 || str[i] > 57)
			return (1);
	}
	return (0);
}

long	ft_atol(char *s)
{
	int		i;
	long	res;

	if (!s)
		return (0);
	res = 0;
	i = -1;
	while (s[++i])
	{
		if (res > (LONG_MAX - (s[i] - '0')) / 10)
			return (-1);
		res = res * 10 + (s[i] - '0');
	}
	return (res);
}
