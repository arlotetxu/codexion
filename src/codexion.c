/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:58:14 by joflorid          #+#    #+#             */
/*   Updated: 2026/03/30 17:01:03 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../inc/codexion.h"
#include<stdio.h>
#include<stdlib.h>


int main(int argc, char **argv)
{
	char	*ret;

	ret = ft_join(argc, argv);
	printf("Retorno: %s\n", ret);
	free(ret);

	return (0);
}
