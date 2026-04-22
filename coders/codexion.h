/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:44:12 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/22 17:23:21 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
//=============LIBRARIES=============
# include <pthread.h>

//=============DEFINITIONS=============
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct s_gen	t_gen;

typedef struct s_params
{
	int				num_coders;
	int				tt_burn;
	int				tt_comp;
	int				tt_deb;
	int				tt_ref;
	int				num_comp_req;
	int				tt_cooldown;
	int				is_edf;
}	t_params;

typedef struct s_priority_q
{
	t_coder			*heap;
	int				size;
	int				capacity;
	int				is_edf;
}	t_priority_q;

typedef struct s_dongle
{
	pthread_mutex_t		m_dongle;
	pthread_cond_t		w_coold;
	long				end_cool;
	int					status;
	t_priority_q		*pq;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	long			st_comp;
	long			prior;
	int				num_comp;
	int				is_burned;
	t_dongle		*left;
	t_dongle		*right;
	pthread_mutex_t	m_coder;
	t_gen			*gen;
}	t_coder;

typedef struct s_gen
{
	t_params		*p;
	t_coder			*c;
	t_dongle		*d;
	pthread_t		watcher;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_gen;
	pthread_mutex_t	m_launch;
	pthread_cond_t	w_align;
	int				stop_sim;
	long			init_time;
	int				launch;
	int				pending_comp;
}	t_gen;

//=============PROTOTYPES//=============
//main
int			ft_start_program(char **all_args, t_params *p_param);
int			ft_start_parsing(char **all_args, t_params *p_param);

//parser.c
int			ft_args_len(int argc, char **argv);
char		*ft_arg_join(int argc, char **argv);
char		*ft_get_word(char **str, char s);
char		**ft_arg_split(char *argv, char sep);
int			ft_num_args(char **all_args);

//parser2.c
int			ft_check_args_nums(char **args);
int			ft_check_arg_int(char **args);
int			ft_check_last_arg(char **args);
int			ft_loading_params(char **all_args, t_params *p_params);
int			ft_start_parsing(char **all_args, t_params *p_param);

//aux.c
int			ft_strlen(char *str);
int			ft_count_words(char *str, char sep);
void		ft_double_free(void *str, char **str2);
int			ft_not_all_digits(char *str);
int			ft_atoi(char *s);

//aux2.c
int			ft_count_args(char **args);
char		*ft_to_lower(char *s);
int			ft_strcmp(char *s1, char *s2);
void		ft_free_gen_struct(t_gen *gen);
long		ft_get_time_ms(void);

//prints.c
int			ft_print_error(int err_nb);
void		ft_print_take_dongle(int id, t_gen *g);
void		ft_print_compiling(t_coder *my_coder);
void		ft_print_debugging(t_coder *my_coder);
void		ft_print_refactoring(t_coder *my_coder);

//init_data.c
t_dongle	*ft_init_dongles(t_params *p_param);
t_coder		*ft_init_coders(t_params *p_params, t_dongle *my_dongles);
t_gen		*ft_init_gen(t_params *p, t_coder *c, t_dongle *d);
t_gen		*ft_start_init_data(t_params *p);

//heap_op.c
void		*ft_add_to_pq(t_coder *my_coder);
void		ft_pq_swap(t_coder *a, t_coder *b);
int			ft_pq_push(t_priority_q *pq, t_coder *new_coder);
void		ft_pq_pop(t_priority_q *pq, int coder_id);
int			ft_pq_initial_push(t_priority_q *pq, t_coder *m);

//create_th.c
int			ft_create_threads(t_gen *g);

//routine.c
void		ft_wait_coders(t_coder *m);
int			ft_exit_routine(t_coder *m);
void		ft_exit_program(t_coder *m);
void		*ft_start_routine(void *arg);

//take_dongles.c
void		ft_sorted_mutex_lock(t_coder *m);
void		ft_sorted_mutex_unlock(t_coder *m);
void		ft_take_dongles(t_coder *my_coder);
void		ft_release_dongles(t_coder *my_coder);
int			ft_can_take_both(t_coder *m);

//compiling.c
void		ft_start_compile(t_coder *my_coder);

//watcher.c
int			ft_create_watcher(t_gen *g);
void		*ft_w_routine(void *arg);
int			ft_check_burnout(t_gen *g);

#endif
