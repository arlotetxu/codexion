/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compiling.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 10:30:53 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/14 15:49:22 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>
#include <unistd.h>

void	ft_start_compile(t_coder *my_coder)
{
	if (my_coder->left->status == 1 && my_coder->right->status == 1)
	{
		ft_print_compiling(my_coder);
		my_coder->num_comp--;
		my_coder->st_comp = ft_get_time_ms();
		my_coder->prior = my_coder->st_comp + my_coder->gen->p->tt_burn;
	}
}

