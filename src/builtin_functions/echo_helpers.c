/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:45:34 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 07:59:50 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*expand_token_value_1(char *value, t_shell *shell, int *i)
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
		var_value = ft_itoa(shell->exit_code);
	else
	{
		env_value = get_env_value(*(shell->env), var_name);
		if (env_value)
			var_value = ft_strdup(env_value);
		else
			var_value = ft_strdup("");
	}
	free(var_name);
	return (var_value);
}

static char	*expand_token_value_1_5(char *dest, const char *to_append)
{
	char	*new;

	new = ft_strjoin(dest, to_append);
	free(dest);
	return (new);
}

static char	*echo_helper(char *result, char *value, t_shell *shell, int *i)
{
	char	*expanded;
	char	*temp;

	if (ft_isalnum(value[i[0]]) || value[i[0]] == '_')
	{
		expanded = expand_token_value_1(value, shell, i);
		temp = result;
		result = ft_strjoin(result, expanded);
		free(temp);
		free(expanded);
	}
	else if (value[i[0]] == '?')
	{
		expanded = ft_itoa(shell->exit_code);
		temp = result;
		result = ft_strjoin(result, expanded);
		free(temp);
		free(expanded);
		i[0]++;
	}
	else
		result = expand_token_value_1_5(result, "$");
	return (result);
}

static char	*expand_token_value_final(char *value, t_shell *shell)
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
			result = echo_helper(result, value, shell, i);
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

t_token	*echo_util(t_token *tokens, t_shell *shell)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD && curr->single_quotes == 0)
		{
			expanded = expand_token_value_final(curr->value, shell);
			free(curr->value);
			curr->value = expanded;
		}
		curr = curr->next;
	}
	return (tokens);
}
