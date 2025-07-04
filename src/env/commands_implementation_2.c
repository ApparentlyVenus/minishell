/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_implementation_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:27:25 by yitani            #+#    #+#             */
/*   Updated: 2025/07/04 23:49:55 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_key(const char *key)
{
	int	i;

	if (!key || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	export_helper(t_env **envp, char *equal_sign, t_token *args)
{
	char	*key;
	char	*value[2];

	key = ft_substr(args->value, 0, equal_sign - args->value);
	value[0] = ft_strdup(equal_sign + 1);
	if (!key || !value[0])
	{
		free(key);
		free(value[0]);
		return ;
	}
	if (!is_valid_key(key))
	{
		free(key);
		free(value[0]);
	}
	else
	{
		value[1] = expand_token_value_final(value[0], *envp);
		set_env_value(envp, key, value[1]);
		free(key);
		free(value[0]);
		free(value[1]);
	}
}

void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	a->key = b->key;
	b->key = temp_key;
	temp_value = a->value;
	a->value = b->value;
	b->value = temp_value;
}

t_env	**do_something(t_env **envp)
{
	t_env	*cur;
	t_env	*head;
	char	*temp_key;
	char	*temp_value;
	int		sorted;

	if (!envp || !*envp)
		return (envp);
	head = *envp;
	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		cur = head;
		while (cur && cur->next)
		{
			if (ft_strcmp(cur->key, cur->next->key) > 0)
			{
				swap_env_nodes(cur, cur->next);
				sorted = 0;
			}
			cur = cur->next;
		}
	}
	return (envp);
}

void	print_sorted_env(t_env **envp)
{
	t_env	*current;

	envp = do_something(envp);
	current = *envp;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

void	builtin_export(t_env **envp, t_token *args)
{
	char	*equal_sign;

	if (!args || !args->next)
	{
		print_sorted_env(*envp);
		return ;
	}
	while (args)
	{
		equal_sign = ft_strchr(args->value, '=');
		if (equal_sign)
			export_helper(envp, equal_sign, args);
		else
			if (is_valid_key(args->value))
				set_env_value(envp, args->value, NULL);
		args = args->next;
	}
}
