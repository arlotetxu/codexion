/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:44:12 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/03 17:43:23 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
//=============LIBRARIES=============
# include <pthread.h>

//=============DEFINITIONS=============
typedef struct s_params
{
	int				num_coders;
	int				tt_burn;
	int				tt_comp;
	int				tt_deb;
	int				tt_ref;
	int				num_comp_req;
	int				tt_cooldown;
	char			*scheduler;
}	t_params;

typedef struct s_coder
{
	int				id;
	int				st_comp;
	int				st_deb;
	int				st_ref;
	int				num_comp;
	int				is_burned;
	int				dongle;
	pthread_mutex_t	mine;
	pthread_mutex_t	right;
	t_params		*p_params;
}	t_coder;

typedef struct s_priority_q
{
	t_coder			*heap;
	int				size;
	pthread_mutex_t	gen;
}	t_priority_q;

//=============PROTOTYPES//=============
//main
int		ft_start_program(char **all_args, t_params *p_param);
int		ft_start_parsing(char **all_args, t_params *p_param);

//parser.c
int		ft_args_len(int argc, char **argv);
char	*ft_arg_join(int argc, char **argv);
char	*ft_get_word(char **str, char s);
char	**ft_arg_split(char *argv, char sep);

//parser2.c
int		ft_num_args(char **all_args);
int		ft_check_args_nums(char **args);
int		ft_check_arg_int(char **args);
int		ft_check_last_arg(char **args);
int		ft_loading_params(char **all_args, t_params *p_params);

//aux.c
int		ft_strlen(char *str);
int		ft_count_words(char *str, char sep);
// void	ft_double_free(char *str, char **str2);
void	ft_double_free(void *str, char **str2);
int		ft_not_all_digits(char *str);
// long	ft_atol(char *s);
int		ft_atoi(char *s);

//aux2.c
int		ft_count_args(char **args);
char	*ft_to_lower(char *s);
int		ft_strcmp(char *s1, char *s2);

//print_error.c
int		ft_print_error(int err_nb);

#endif