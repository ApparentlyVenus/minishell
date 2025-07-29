/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phases.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:13:30 by odana             #+#    #+#             */
/*   Updated: 2025/07/29 04:11:20 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	expand_single_quoted_arg(t_arg *arg)
{
	(void)arg;
}

static void	expand_double_quoted_arg(t_arg *arg, t_shell *shell,
			t_builtin builtin_type, int index)
{
	char	*expanded;

	expanded = expand_cmd_arg(arg->value, shell, builtin_type, index);
	if (expanded && expanded != arg->value)
	{
		free(arg->value);
		arg->value = expanded;
	}
}

static void	expand_unquoted_arg(t_arg *arg, t_shell *shell,
			t_builtin builtin_type, int index)
{
	char	*expanded;
	char	*unquoted;

	expanded = expand_cmd_arg(arg->value, shell, builtin_type, index);
	if (expanded && expanded != arg->value)
	{
		free(arg->value);
		arg->value = expanded;
	}
	unquoted = remove_all_quotes(arg->value);
	if (unquoted)
	{
		free(arg->value);
		arg->value = unquoted;
	}
}

void	expand_variables_phase(t_cmd *cmd,
	t_shell *shell, t_builtin builtin_type)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i]->single_quotes)
			expand_single_quoted_arg(cmd->args[i]);
		else if (cmd->args[i]->double_quotes)
			expand_double_quoted_arg(cmd->args[i], shell, builtin_type, i);
		else
			expand_unquoted_arg(cmd->args[i], shell, builtin_type, i);
		i++;
	}
}

void	expand_splitting_phase(t_arg ***args)
{
	int	i;
	int	added_args;

	i = 0;
	while ((*args)[i])
	{
		if ((*args)[i]->single_quotes || (*args)[i]->double_quotes)
			continue ;
		if (i > 0 && (*args)[0] && ft_strcmp((*args)[0]->value, "export") == 0
			&& is_assignment((*args)[i]->value))
		{
			i++;
			continue ;
		}
		added_args = perform_word_split(args, i);
		if (added_args > 0)
		{
			i += added_args + 1;
			continue ;
		}
		added_args = perform_wildcard_expand(args, i);
		i += added_args + 1;
	}
}
