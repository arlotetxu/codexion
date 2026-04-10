/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:18:34 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/10 13:43:42 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>


int	ft_create_threads(t_gen *g)
{
	int	i;
	pthread_t	th[g->p->num_coders];

	i = -1;
	while (++i < g->p->num_coders)
		if(pthread_create(&th[i], NULL, ft_start_routine(), g->c + i) != 0)
			return (-1);
	i = -1;
	while (++i < g->p->num_coders)
		if (pthread_join(th[i], NULL) != 0)
			return (-1);
	//pthread_destroy
	//pthread_mutex_destroy
	return (0);
}
