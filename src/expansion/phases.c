/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phases.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:13:30 by odana             #+#    #+#             */
/*   Updated: 2025/08/01 02:26:41 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	char	*tilde_expanded;

	expanded = expand_cmd_arg(arg->value, shell, builtin_type, index);
	if (expanded && expanded != arg->value)
	{
		free(arg->value);
		arg->value = expanded;
	}
	if (builtin_type != BUILTIN_CD)
	{
		tilde_expanded = expand_tilde(arg->value, shell->env);
		if (tilde_expanded && tilde_expanded != arg->value)
		{
			free(arg->value);
			arg->value = tilde_expanded;
		}
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
		{
			i++;
			continue ;
		}
		else if (cmd->args[i]->double_quotes)
			expand_double_quoted_arg(cmd->args[i], shell, builtin_type, i);
		else
			expand_unquoted_arg(cmd->args[i], shell, builtin_type, i);
		i++;
	}
	concat_args(cmd);
}

static int	skip_expansion_conditions(t_arg **args, int i)
{
	if (args[i]->single_quotes || args[i]->double_quotes)
		return (1);
	if (i > 0 && args[0] && ft_strcmp(args[0]->value, "export") == 0
		&& is_assignment(args[i]->value))
		return (1);
	return (0);
}

void	expand_splitting_phase(t_arg ***args)
{
	int	i;
	int	expanded;

	i = 0;
	while ((*args)[i])
	{
		if (skip_expansion_conditions(*args, i))
		{
			i++;
			continue ;
		}
		expanded = perform_word_split(args, i);
		if (expanded > 0)
		{
			i += expanded + 1;
			continue ;
		}
		expanded = perform_wildcard_expand(args, i);
		if (expanded > 0)
		{
			i += expanded + 1;
			continue ;
		}
		i++;
	}
}
