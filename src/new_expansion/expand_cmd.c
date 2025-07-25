/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:11 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 08:27:10 by odana            ###   ########.fr       */
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
	int		i;
	char	*expanded;
	char	*original;

	if (!cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		original = cmd->args[i]->value;
		if (cmd->args[i]->single_quotes)
		{
			i++;
			continue ;
		}
		expanded = expand_cmd_arg(original, shell, builtin_type, i);
		if (expanded && expanded != original)
		{
			free(original);
			cmd->args[i]->value = expanded;
		}
		i++;
	}
}

void	expand_cmd_redirs(t_cmd *cmd, t_env *env)
{
	t_redir	*redir;
	char	*expanded;

	redir = cmd->redirs;
	while (redir)
	{
		expanded = NULL;
		if (redir->filename)
		{
			expanded = expand_variables(redir->filename, env);
			if (expanded && expanded != redir->filename)
			{
				free(redir->filename);
				redir->filename = expanded;
			}
		}
		redir = redir->next;
	}
}
