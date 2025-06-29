/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_environment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:06:38 by yitani            #+#    #+#             */
/*   Updated: 2025/06/29 20:29:48 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_entries_count(t_env *env)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env;
	while (current)
	{
		if (current->equal == 1)
			count++;
		current = current->next;
	}
	return (count);
}

char	**to_envp(t_env *env)
{
	int		count;
	char	**envp;
	int		i;
	char	*temp[2];
	t_env	*curr;

	i = 0;
	count = valid_entries_count(env);
	curr = env;
	envp = malloc(sizeof(char *) * (count + 1));
	while (i < count && env)
	{
		if (env->equal == 1)
		{
			temp[0] = ft_strjoin(env->key, "=");
			temp[1] = ft_strjoin(temp[0], env->value);
			envp[i] = temp[1];
			free(temp[0]);
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return(envp);
}

// var[0] = var_name
// var[1] = var_value

char	*expand_token_value_1(char *value, t_env *env, int *i)
{
	int		start;
	char	*var_name;
	char	*var_value;

	start = i[0];
	while (ft_isalnum(value[i[0]] || value[i[0]] == '_'))
		i[0]++;
	var_name = ft_substr(value, i[1], i[0] - i[1]);
	var_value = get_env_value(env, var_name);
	if (!var_value)
		var_value = ft_strdup("");
	else
		var_value = ft_strdup(var_value);
	free(var_name);
	return(var_value);
}

char	*expand_token_value_1_5(char *dest, const char *to_append)
{
	char	*new;

	new = ft_strjoin(dest, to_append);
	free(dest);
	return (new);
}

char	*handle_dollar_expansion(char *result, char *value, t_env *env, int *i)
{
	char *expanded;
	char *temp;

	if (ft_isalnum(value[i[0]]) || value[i[0]] == '_')
	{
		expanded = expand_token_value_1(value, env, i);
		temp = result;
		result = ft_strjoin(result, expanded);
		free(temp);
		free(expanded);
	}
	else
	{
		result = expand_token_value_1_5(result, "$");
	}
	return (result);
}

char	*expand_token_value_final(char *value, t_env *env)
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
			result = ft_strjoin(result, expanded);
			free(temp);
			free(expanded);
		}
	}
	return (result);
}

t_token	*expand_variables_in_tokens(t_token *tokens, t_env *env)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD && curr->single_quotes == 0)
		{
			expanded = expand_token_value_final(curr->value, env);
			free(curr->value);
			curr->value = expanded;
		}
		curr = curr->next;
	}
	return (tokens);
}
