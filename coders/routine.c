/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 12:24:18 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/10 12:59:40 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_start_routine(void *arg)
{
	t_coder	*my_coder;

	my_coder = (t_coder *)arg;
	printf("Iniciando rutina con %i\n", my_coder->id);
	return (0);
}
