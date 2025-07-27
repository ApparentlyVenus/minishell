/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:05:38 by yitani            #+#    #+#             */
/*   Updated: 2025/07/27 13:25:29 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_new_key(t_env **env, char *key, char *value)
{
	t_env	*new_key;

	if (!key)
		return ;
	else
	{
		new_key = malloc(sizeof(t_env));
		new_key->key = ft_strdup(key);
		if (!value)
			new_key->value = NULL;
		else
			new_key->value = ft_strdup(value);
		new_key->equal = (value != NULL);
		new_key->next = NULL;
		env_add_back(env, new_key);
	}
}

void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;

	current = *env;
	if (!key)
		return ;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->equal = (value != NULL);
			free(current->value);
			if (!value)
				current->value = NULL;
			else
				current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	handle_new_key(env, key, value);
}

void	export_helper(t_env **envp, char *equal_sign, char *args, int *failure)
{
	char	*key;
	char	*value;

	key = ft_substr(args, 0, equal_sign - args);
	value = ft_strdup(equal_sign + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return ;
	}
	if (!is_valid_key(key))
	{
		free(key);
		free(value);
		failure[0] = 1;
	}
	else
	{
		set_env_value(envp, key, value);
		free(key);
		free(value);
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
