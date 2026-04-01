/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:44:12 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/01 17:05:47 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
//=============LIBRARIES=============



//=============DEFINITIONS=============
typedef struct	s_params
{
	int		num_coders;
	long	tt_burn;
	long	tt_comp;
	long	tt_deb;
	long	tt_ref;
	int		num_comp_req;
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
	int			is_burned;
	t_params	*p_params;
}	t_coder;


//=============PROTOTYPES//=============
//main
int	ft_start_parsing(char *arg_join, char **all_args);

//parser.c
int		ft_args_len(int argc, char **argv);
char	*ft_arg_join(int argc, char **argv);
char	*ft_get_word(char **str, char s);
char	**ft_arg_split(char *argv, char sep);

//parser2.c
int		ft_num_args(char **all_args);
int		ft_check_args_nums(char **args);
int		ft_check_arg_long(char **args);
int		ft_check_last_arg(char **args);


//aux.c
int		ft_strlen(char *str);
int		ft_count_words(char *str, char sep);
void	ft_double_free(char *str, char **str2);
int		ft_not_all_digits(char *str);
long	ft_atol(char *s);

//aux2.c
int		ft_count_args(char **args);
char	*ft_to_lower(char *s);


//print_error.c
int		ft_print_error(int err_nb);

# endif
