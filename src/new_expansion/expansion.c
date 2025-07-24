/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:13 by odana             #+#    #+#             */
/*   Updated: 2025/07/24 21:51:25 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	expand_variables_in_args(char **args, t_env *env)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args && args[i])
	{
		expanded = expand_variables(args[i], env);
		if (expanded && expanded != args[i])
		{
			free(args[i]);
			args[i] = expanded;
		}
		i++;
	}
}

void	word_split_args(char ***args)
{
	int		i;
	char	**split;

	i = 0;
	while (*args && (*args)[i])
	{
		if (contains_whitespace((*args)[i]))
		{
			split = ft_split((*args)[i], ' ');
			if (!split || !split[0])
			{
				free_split(split);
				i++;
				continue ;
			}
			i += replace_args_with_split(args, split, i);
		}
		i++;
	}
}

int	replace_args_with_matches(char ***args, char **matches, int i)
{
	char	**new_args;
	int		old_count;
	int		match_count;
	int		j;

	old_count = count_args(*args);
	match_count = count_args(matches);
	new_args = malloc(sizeof(char *) * (old_count + match_count));
	if (!new_args)
		return (0);
	j = copy_args_before(new_args, *args, i);
	j = copy_split_args(new_args, matches, j);
	j = copy_args_after(new_args, *args, i, j);
	new_args[j] = NULL;
	free((*args)[i]);
	free(*args);
	*args = new_args;
	return (match_count - 1);
}

void	expand_wildcards_in_args(char ***args)
{
	int		i;
	char	**matches;
	int		match_count;

	i = 0;
	while (*args && (*args)[i])
	{
		matches = NULL;
		match_count = wildcard_expand((*args)[i], &matches);
		if (match_count > 0)
			i += replace_args_with_matches(args, matches, i);
		else if (matches)
			free(matches);
		i++;
	}
}

void	expand_all_args(char ***args, t_env *env)
{
	expand_variables_in_args(*args, env);
	word_split_args(args);
	expand_wildcards_in_args(args);
}
