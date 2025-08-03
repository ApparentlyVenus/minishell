/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:00:56 by odana             #+#    #+#             */
/*   Updated: 2025/08/03 18:39:51 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_assignment(char *arg)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	if (eq == arg)
		return (0);
	return (1);
}

char	*join_assignment(char *var, char *val)
{
	char	*tmp;
	char	*result;

	result = ft_strjoin(var, "=");
	tmp = result;
	if (result)
	{
		result = ft_strjoin(result, val);
		free(tmp);
	}
	return (result);
}

int	contains_whitespace(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}

void	process_single_redir(t_redir *current, t_env *env)
{
	char	*expanded;
	char	*unquoted;
	char	*tilde_expanded;

	expanded = NULL;
	if (!current->filename)
		return ;
	expanded = expand_variables(current->filename, env);
	if (expanded && expanded != current->filename)
		set_filename(current, expanded);
	if (!current->single_quotes && !current->double_quotes)
	{
		tilde_expanded = expand_tilde(current->filename, env);
		if (tilde_expanded && tilde_expanded != current->filename)
			set_filename(current, tilde_expanded);
	}
	unquoted = remove_all_quotes(current->filename);
	if (unquoted)
		set_filename(current, unquoted);
}
