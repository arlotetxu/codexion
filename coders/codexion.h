/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:44:12 by joflorid          #+#    #+#             */
/*   Updated: 2026/03/31 12:49:38 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
//=============LIBRARIES=============
#include <stdbool.h>



//=============DEFINITIONS=============
typedef struct	s_params
{
	int		num_coders;
	long	tt_burn;
	long	tt_comp;
	long	tt_deb;
	long	tt_ref;
	int		num_comp;
	long	tt_cooldown;
	char	*scheduler;
}	t_params;

typedef struct	s_coder
{
	int			id;
	long		st_comp;
	long		st_deb;
	long		st_ref;
	int			num_comp;
	bool		is_burned;
	t_params	*p_params;
}	t_coder;


//=============PROTOTYPES//=============
//parser.c
int		ft_args_len(int argc, char **argv);
char	*ft_arg_join(int argc, char **argv);
char	*ft_get_word(char **str, char s);
char	**ft_arg_split(char *argv, char sep);


//aux.c
int		ft_strlen(char *str);
int		ft_count_words(char *str, char sep);
void	ft_double_free(char **str);


# endif
