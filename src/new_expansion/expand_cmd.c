/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:11 by odana             #+#    #+#             */
/*   Updated: 2025/07/13 18:58:12 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** is_assignment
** Purpose: Checks if a string is an assignment (VAR=VAL).
** Used variables: arg (input string)
** Return: 1 if assignment, 0 otherwise
*/
int is_assignment(char *arg)
{
	char *eq;
	eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	if (eq == arg)
		return (0);
	return (1);
}

/*
** join_assignment
** Purpose: Joins variable and value with '=' for assignment expansion.
** Used variables: var (left), val (right)
** Return: Newly allocated string "var=val"
*/
char *join_assignment(char *var, char *val)
{
	char *tmp;
	char *result;

	result = ft_strjoin(var, "=");
	tmp = result;
	if (result)
	{
		result = ft_strjoin(result, val);
		free(tmp);
	}
	return (result);
}

/*
** expand_assignment_value
** Purpose: Expands only the right side of an assignment (VAR=VAL).
** Used variables: arg (input), env (env list)
** Return: Newly allocated expanded assignment string
*/
char *expand_assignment_value(char *arg, t_env *env)
{
	char *eq;
	char *var;
	char *val;
	char *result;
	int var_len;

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

/*
** expand_cmd_arg
** Purpose: Expands a single argument in t_cmd according to shell rules.
** Used variables: arg (argument), env (env list), builtin_type, index
** Return: Newly allocated expanded string
*/
char *expand_cmd_arg(char *arg, t_env *env, t_builtin builtin_type, int index)
{
	char *expanded;

	expanded = NULL;
	if (builtin_type == BUILTIN_EXPORT && is_assignment(arg))
		expanded = expand_assignment_value(arg, env);
	else if (is_assignment(arg) && index == 0)
		expanded = expand_assignment_value(arg, env);
	else
		expanded = expand_variables(arg, env);
	return (expanded);
}

/*
** expand_cmd_args
** Purpose: Expands all arguments in t_cmd in place.
** Used variables: cmd (command), env (env list), builtin_type
** Return: None (modifies cmd->args in place)
*/
void expand_cmd_args(t_cmd *cmd, t_env *env, t_builtin builtin_type)
{
	int i;
	char *expanded;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		expanded = expand_cmd_arg(cmd->args[i], env, builtin_type, i);
		if (expanded && expanded != cmd->args[i])
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
}

/*
** expand_cmd_redirs
** Purpose: Expands all redirection filenames in t_cmd in place.
** Used variables: cmd (command), env (env list)
** Return: None (modifies cmd->redirs in place)
*/
void expand_cmd_redirs(t_cmd *cmd, t_env *env)
{
	t_redir *redir;
	char *expanded;

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

/*
** expand_cmd
** Purpose: Expands arguments and redirections in t_cmd for JIT shell expansion.
** Used variables: cmd (command), env (env list), builtin_type
** Return: None (modifies cmd in place)
*/
void expand_cmd(t_cmd *cmd, t_env *env, t_builtin builtin_type)
{
	if (!cmd)
		return ;
	expand_cmd_args(cmd, env, builtin_type);
	expand_cmd_redirs(cmd, env);
} 