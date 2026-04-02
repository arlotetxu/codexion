/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:08:30 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/02 19:22:00 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>

int	ft_count_args(char **args)
{
	int	count;

	if (!args)
		return (0);
	count = 0;
	while (args[count])
		count++;
	return (count);
}

char	*ft_to_lower(char *s)
{
	int		i;
	char	*ret;

	if (!s)
		return (NULL);
	ret = malloc(sizeof(s));
	if (!ret)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] >= 65 && s[i] <= 90)
			ret[i] = s[i] + 32;
		else if ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
			continue ;
		else
			ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}
