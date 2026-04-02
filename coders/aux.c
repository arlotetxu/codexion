/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:16:09 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/02 19:20:04 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
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
		if ((str[i] >= 48 && str[i] <= 57) || str[i] == '+')
			i++;
		else
			return (1);
	}
	return (0);
}

// long	ft_atol(char *s)
// {
// 	int		i;
// 	long	res;

// 	if (!s)
// 		return (0);
// 	res = 0;
// 	i = -1;
// 	while (s[++i])
// 	{
// 		if (res > (LONG_MAX - (s[i] - '0')) / 10)
// 			return (-1);
// 		res = res * 10 + (s[i] - '0');
// 	}
// 	return (res);
// }

int	ft_atoi(char *s)
{
	int	i;
	int	res;

	res = 0;
	i = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	while (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			return (-1);
		i++;
	}
	while (s[i] >= 48 && s[i] <= 57)
	{
		if (res > (INT_MAX - (s[i] - '0')) / 10)
			return (-1);
		res = res * 10 + (s[i] - '0');
		i++;
	}
	return (res);
}
