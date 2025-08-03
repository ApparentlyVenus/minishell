/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:12:59 by odana             #+#    #+#             */
/*   Updated: 2025/08/01 03:22:23 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_assignment_value(char *arg, t_env *env)
{
	char	*eq;
	char	*var;
	char	*val;
	char	*result;
	int		var_len;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (ft_strdup(arg));
	var_len = eq - arg;
	var = ft_substr(arg, 0, var_len);
	val = expand_variables(eq + 1, env);
	result = NULL;
	if (var && val)
		result = join_assignment(var, val);
	if (var)
		free(var);
	if (val)
		free(val);
	if (!result)
		result = ft_strdup(arg);
	return (result);
}

char	*expand_cmd_arg(char *arg, t_shell *shell, t_builtin builtin_type,
	int index)
{
	char	*expanded;

	expanded = NULL;
	if (builtin_type == BUILTIN_EXPORT && is_assignment(arg))
		expanded = expand_assignment_value(arg, shell->env);
	else if (is_assignment(arg) && index == 0)
		expanded = expand_assignment_value(arg, shell->env);
	else
		expanded = expand_exit(arg, shell->env, shell->exit_code);
	return (expanded);
}

void	expand_cmd_args(t_cmd *cmd, t_shell *shell, t_builtin builtin_type)
{
	if (!cmd || !cmd->args)
		return ;
	expand_variables_phase(cmd, shell, builtin_type);
	expand_splitting_phase(&cmd->args);
}

void	expand_cmd_redirs(t_cmd *cmd, t_env *env)
{
	char	*expanded;
	char	*unquoted;
	char	*tilde_expanded;

	while (cmd->redirs)
	{
		expanded = NULL;
		if (cmd->redirs->filename)
		{
			expanded = expand_variables(cmd->redirs->filename, env);
			if (expanded && expanded != cmd->redirs->filename)
				set_filename(cmd->redirs, expanded);
			if (!cmd->redirs->single_quotes && !cmd->redirs->double_quotes)
			{
				tilde_expanded = expand_tilde(cmd->redirs->filename, env);
				if (tilde_expanded && tilde_expanded != cmd->redirs->filename)
					set_filename(cmd->redirs, tilde_expanded);
			}
			unquoted = remove_all_quotes(cmd->redirs->filename);
			if (unquoted)
				set_filename(cmd->redirs, unquoted);
		}
		cmd->redirs = cmd->redirs->next;
	}
}

void	expand_cmd(t_cmd *cmd, t_builtin type, t_shell *shell)
{
	if (!cmd)
		return ;
	expand_cmd_args(cmd, shell, type);
	expand_splitting_phase(&cmd->args);
	expand_cmd_redirs(cmd, shell->env);
}
