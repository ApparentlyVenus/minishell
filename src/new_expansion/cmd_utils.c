/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:00:56 by odana             #+#    #+#             */
/*   Updated: 2025/07/24 21:50:39 by odana            ###   ########.fr       */
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

void	expand_cmd(t_cmd *cmd, t_builtin type, t_shell *shell)
{
	if (!cmd)
		return ;
	expand_cmd_args(cmd, shell, type);
	expand_cmd_redirs(cmd, shell->env);
}
