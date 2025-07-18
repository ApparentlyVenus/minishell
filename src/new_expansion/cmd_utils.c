/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:00:56 by odana             #+#    #+#             */
/*   Updated: 2025/07/18 13:06:47 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** is_assignment
** Purpose: Checks if a string is an assignment (VAR=VAL).
** Used variables: arg (input string)
** Return: 1 if assignment, 0 otherwise
*/
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

/*
** join_assignment
** Purpose: Joins variable and value with '=' for assignment expansion.
** Used variables: var (left), val (right)
** Return: Newly allocated string "var=val"
*/
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

/*
** expand_cmd
** Purpose: Expands arguments and redirections in t_cmd for JIT shell expansion.
** Used variables: cmd (command), env (env list), builtin_type
** Return: None (modifies cmd in place)
*/
void	expand_cmd(t_cmd *cmd, t_env *env, t_builtin builtin_type)
{
	if (!cmd)
		return ;
	expand_cmd_args(cmd, env, builtin_type);
	expand_cmd_redirs(cmd, env);
}
