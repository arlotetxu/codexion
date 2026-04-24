/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:00:50 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/24 17:03:12 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"
#include <stdlib.h>

int	ft_args_len(int argc, char **argv)
{
	int	i;
	int	j;
	int	len;

	len = 0;
	i = 0;
	while (argv[++i])
	{
		j = -1;
		while (argv[i][++j])
			len++;
	}
	return (len + argc - 1);
}

char	*ft_arg_join(int argc, char **argv)
{
	int		i;
	int		j;
	int		k;
	char	*all_argv;

	k = 0;
	i = 0;
	all_argv = malloc(sizeof(char) * (ft_args_len(argc, argv) + 1));
	if (!all_argv)
		return (NULL);
	while (argv[++i])
	{
		j = -1;
		while (argv[i][++j])
			all_argv[k++] = argv[i][j];
		if (i < argc - 1)
			all_argv[k++] = ' ';
	}
	all_argv[k] = '\0';
	return (all_argv);
}

char	*ft_get_word(char **str, char s)
{
	int		len;
	char	*word;
	int		i;

	while (**str == s)
		(*str)++;
	len = 0;
	while ((*str)[len] && (*str)[len] != s)
		len++;
	if (len == 0)
		return (NULL);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = -1;
	while (++i < len)
	{
		word[i] = **str;
		(*str)++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_arg_split(char *argv, char sep)
{
	char	**ret;
	int		words;
	int		i;

	words = ft_count_words(argv, sep);
	ret = malloc(sizeof(char *) * (words + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (i < words)
	{
		ret[i] = ft_get_word(&argv, sep);
		if (!ret[i])
		{
			ft_double_free(NULL, ret);
			return (NULL);
		}
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

int	ft_num_args(char **all_args)
{
	int	num_args;

	num_args = 0;
	while (all_args[num_args])
		num_args++;
	return (num_args);
}
