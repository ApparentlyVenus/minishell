/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:45:34 by yitani            #+#    #+#             */
/*   Updated: 2025/07/11 16:20:47 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_tilde(char *value, t_env *env)
{
	char	*result;
	char	*home;
	char	*suffix;
	
	if (!value)
		return (NULL);
	if (value[0] == '~' && (value[1] == '/' || value[1] == '\0'))
	{
		home = get_env_value(env, "HOME");
		if (!home)
			return (ft_strdup(value));
		suffix = ft_strdup(value + 1);
		if (!suffix)
			return (ft_strdup(value));
		result = ft_strjoin(home, suffix);
		free(suffix);
		if (!result)
			return (ft_strdup(value));
		return (result);
	}
	return (ft_strdup(value));
}

/*
 * extract_and_expand_variable - Extracts variable name and returns its value
 * 
 * @value: String containing variable reference
 * @shell: Shell execution context
 * @i: Current position index (will be modified)
 * @return: Expanded variable value
 */
char	*extract_variable(char *value, t_env *env, int *i)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*env_value;

	start = i[0];
	while (ft_isalnum(value[i[0]]) || value[i[0]] == '_')
		i[0]++;
	if (i[0] == start)
		i[0]++;
	var_name = ft_substr(value, start, i[0] - start);
	if (ft_strcmp(var_name, "?") == 0)
		var_value = ft_strdup("0");
	else
	{
		env_value = get_env_value(env, var_name);
		if (env_value)
			var_value = ft_strdup(env_value);
		else
			var_value = ft_strdup("");
	}
	free(var_name);
	return (var_value);
}

/*
 * handle_dollar_expansion - Handles different types of $ expansions
 * 
 * - result: Current result string
 * 
 * - value: String being processed
 * 
 * - shell: Shell execution context
 * 
 * - i: Current position index (will be modified)
 * 
 * @return Updated result string
 */
char	*handle_dollar_expansion(char *result, char *value, t_env *env, int *i)
{
	char	*expanded;
	char	*temp;

	if (ft_isalnum(value[i[0]]) || value[i[0]] == '_')
	{
		expanded = extract_variable(value, env, i);
		temp = result;
		result = ft_strjoin(result, expanded);
		free(temp);
		free(expanded);
	}	
	else if (value[i[0]] == '?')
	{
		expanded = ft_strdup("0");
		temp = result;
		result = ft_strjoin(result, expanded);
		free(temp);
		free(expanded);
		i[0]++;
	}
	else
		result = append_string(result, "$");
	return (result);
}

// ! THE ABOVE FUNCTION WAS THIS ECHO_HELPER FUNCTION

// char    *echo_helper(char *result, char *value, t_exec *shell, int *i)
// {
//     char    *expanded;
//     char    *temp;

//     if (ftisalnum(value[i[0]]) || value[i[0]] == '')
//     {
//         expanded = expand_token_value_1(value, shell, i);
//         temp = result;
//         result = ft_strjoin(result, expanded);
//         free(temp);
//         free(expanded);
//     }
//     else if (value[i[0]] == '?')
//     {
//         expanded = ft_itoa(shell->exit_code);
//         temp = result;
//         result = ft_strjoin(result, expanded);
//         free(temp);
//         free(expanded);
//         i[0]++;
//     }
//     else
//         result = expand_token_value_1_5(result, "$");
//     return (result);
// }

/*
 * expand_variables_in_string - Expands all $ variables in a string
 * 
 * - value: String containing potential $ variables
 * 
 * - shell: Shell execution context
 * 
 * @return New string with variables expanded
 */
char	*expand_variables(char *value, t_env *env)
{
	int		i[2];
	char	*result;
	char	*temp;
	char	*expanded;

	i[0] = 0;
	result = ft_strdup("");
	while (value[i[0]])
	{
		if (value[i[0]++] == '$')
			result = handle_dollar_expansion(result, value, env, i);
		else
		{
			i[1] = i[0];
			while (value[i[0]] && value[i[0]] != '$')
				i[0]++;
			expanded = ft_substr(value, i[1], i[0] - i[1]);
			temp = result;
			result = ft_strjoin(temp, expanded);
			free(temp);
			free(expanded);
		}
	}
	return (result);
}
