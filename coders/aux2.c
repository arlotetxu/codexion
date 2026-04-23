/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:08:30 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/23 11:29:38 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
	int		j;
	char	*ret;

	if (!s)
		return (NULL);
	ret = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] >= 65 && s[i] <= 90)
			ret[j] = s[i] + 32;
		else if (s[i] >= 'a' && s[i] <= 'z')
			ret[j] = s[i];
		i++;
		j++;
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

void	ft_free_gen_struct(t_gen *gen)
{
	int	i;

	i = 0;
	while (i < gen->p->num_coders)
	{
		pthread_mutex_destroy(&gen->d[i].m_dongle);
		pthread_cond_destroy(&gen->d[i].w_coold);
		pthread_mutex_destroy(&gen->c[i].m_coder);
		free(gen->d[i].pq->heap);
		free(gen->d[i].pq);
		i++;
	}
	pthread_mutex_destroy(&gen->m_gen);
	pthread_mutex_destroy(&gen->m_launch);
	pthread_mutex_destroy(&gen->m_print);
	pthread_cond_destroy(&gen->w_align);
	free(gen->d);
	free(gen->c);
	free(gen->p);
	free(gen);
}

long	ft_get_time_ms(void)
{
	struct timeval	tv;
	long			time_ms;

	gettimeofday(&tv, NULL);
	time_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_ms);
}
