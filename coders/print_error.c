/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 13:07:40 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/02 19:17:45 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

int	ft_print_error(int err_nb)
{
	if (err_nb == 1)
		printf("[ERROR]-Wrong number of arguments.\n");
	else if (err_nb == 2)
		printf("[ERROR]-First 7 arguments must be numeric and positives "
			"numbers (without signs).\n");
	else if (err_nb == 3)
		printf("[ERROR]-Arguments must be within long limits "
			"(max: 9223372036854775807).\n");
	else if (err_nb == 4)
		printf("[ERROR]-Last argument not valid (valid options: fifo/edf).\n");
	return (err_nb);
}
