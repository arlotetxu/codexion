/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 10:04:55 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/16 10:11:48 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	ft_sleep_ms(long ms)
{
	long	i;

	i = ft_get_time_ms();
	while ((ft_get_time_ms() - i) < ms)
		usleep(100);
}
