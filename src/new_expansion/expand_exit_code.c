/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_exit_code.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:38:38 by odana             #+#    #+#             */
/*   Updated: 2025/07/24 23:34:14 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_exit(char *str, t_env *env, int exit_code)
{
	char	*result;
	char	*temp;
	char	*exit_str;

	if (!str)
		return (NULL);
	result = expand_variables(str, env);
	if (!result)
		return (ft_strdup(str));
	if (ft_strnstr(result, "$?", ft_strlen(result)))
	{
		exit_str = ft_itoa(exit_code);
		if (!exit_str)
			return (result);
		temp = replace_exit_code(result, exit_str);
		free(exit_str);
		free(result);
		if (!temp)
			return (ft_strdup(str));
		result = temp;
	}
	return (result);
}

char	*replace_exit_code(char *str, char *exit_code_str)
{
	char	*result;
	char	*pos;
	char	*new_str;

	result = ft_strdup(str);
	if (!result)
		return (NULL);
	pos = ft_strnstr(result, "$?", ft_strlen(result));
	while (pos)
	{
		new_str = replace_substring(result, pos, "$?", exit_code_str);
		free(result);
		result = new_str;
		if (!result)
			return (NULL);
		pos = ft_strnstr(result, "$?", ft_strlen(result));
	}
	return (result);
}

char	*replace_substring(char *str, char *pos, char *old_str, char *new_str)
{
	char	*before;
	char	*after;
	char	*temp;
	char	*result;

	before = ft_substr(str, 0, pos - str);
	after = ft_strdup(pos + ft_strlen(old_str));
	if (!before || !after)
		return (free(before), free(after), NULL);
	temp = ft_strjoin(before, new_str);
	free(before);
	if (!temp)
		return (free(after), NULL);
	result = ft_strjoin(temp, after);
	free(temp);
	free(after);
	return (result);
}
