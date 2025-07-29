/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:00:56 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 22:18:36 by odana            ###   ########.fr       */
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
