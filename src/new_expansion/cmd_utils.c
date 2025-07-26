/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:00:56 by odana             #+#    #+#             */
/*   Updated: 2025/07/26 11:40:44 by odana            ###   ########.fr       */
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

char	*remove_quotes(char *filename)
{
	char	*result;
	int		len;

	len = ft_strlen(filename);
	if (!filename)
		return (NULL);
	if (len >= 2 && filename[0] == '\'' && filename[len - 1] == '\'')
	{
		result = ft_substr(filename, 1, len -2);
		return (result);
	}
	if (len >= 2 && filename[0] == '"' && filename[len - 1] == '"')
	{
		result = ft_substr(filename, 1, len -2);
		return (result);
	}
	if (ft_strchr(filename, '"') || ft_strchr(filename, '\''))
	{
		result = remove_adjacent_quotes(filename);
		return (result);
	}
	return (result);
}

char	*remove_adjacent_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (ft_strdup(str));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			i++;
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
