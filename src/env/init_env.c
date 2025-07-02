/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:32:22 by yitani            #+#    #+#             */
/*   Updated: 2025/06/30 21:48:07 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	env_init(t_env **env_list, char **envp)
{
	int	i;
	t_env	*new_node;
	char	*equal_sign;

	i = -1;
	new_node = NULL;
	while (envp[++i])
	{
		new_node = malloc(sizeof(t_env));
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			new_node->equal = 1;
			new_node->value = ft_strdup(equal_sign + 1);
			new_node->key = ft_substr(envp[i], 0, equal_sign - envp[i]);
		}
		else
		{
			new_node->equal = 0;
			new_node->value = NULL;
			new_node->key = envp[i];
		}
		new_node->next = NULL;
		ft_lstadd_back(env_list, new_node);
	}
}

char	*get_env_value(t_env *env, char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	handle_new_key(t_env *env, char *key, char *value)
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
		ft_lstadd_back(&env, new_key);
	}
}

void	set_env_value(t_env *env, char *key, char *value)
{
	t_env	*new_key;

	if (!key)
		return ;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			env->equal = (value != NULL);
			free(env->value);
			if (!value)
				env->value = NULL;
			else
				env->value = ft_strdup(value);
			return ;
		}
		env = env->next;
	}
	handle_new_key(env, key, value);
}

void	unset_env_value(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	if (!env || !*env || !key)
		return ;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev == NULL)
				*env = curr->next;
			else
				prev->next = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
