/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 23:27:39 by odana             #+#    #+#             */
/*   Updated: 2025/07/29 04:09:40 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*temp;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			temp = expand_single_var(str, &i, env);
			result = append_to_result(result, temp);
			free(temp);
		}
		else
		{
			temp = char_to_string(str[i]);
			result = append_to_result(result, temp);
			free(temp);
			i++;
		}
	}
	return (result);
}

char	*expand_single_var(char *str, int *pos, t_env *env)
{
	char	*var_name;
	char	*value;
	int		start;

	(*pos)++;
	start = *pos;
	if (str[*pos] == '?')
	{
		(*pos)++;
		return (ft_strdup("$?"));
	}
	while (str[*pos] && is_var_char(str[*pos]))
		(*pos)++;
	if (*pos == start)
		return (ft_strdup("$"));
	var_name = ft_substr(str, start, *pos - start);
	if (!var_name)
		return (ft_strdup(""));
	value = get_env_value(env, var_name);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*char_to_string(char c)
{
	char	*result;

	result = malloc(2);
	if (!result)
		return (NULL);
	result[0] = c;
	result[1] = '\0';
	return (result);
}

char	*append_to_result(char *result, char *to_append)
{
	char	*new_result;

	if (!result)
	{
		if (to_append)
			return (ft_strdup(to_append));
		return (ft_strdup(""));
	}
	if (!to_append)
		return (result);
	new_result = ft_strjoin(result, to_append);
	free(result);
	return (new_result);
}
