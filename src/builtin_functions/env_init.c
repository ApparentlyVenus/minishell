/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:41:31 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 01:46:56 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	cleanup_and_exit(t_env **env_list, t_env *new_node)
{
	if (new_node)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
	}
	free_env(*env_list);
	*env_list = NULL;
}

static int	init_new_node(t_env *new_node)
{
	new_node->key = NULL;
	new_node->value = NULL;
	new_node->equal = 0;
	new_node->next = NULL;
	return (1);
}

static int	process_env_entry(t_env *new_node, char *envp_entry)
{
	char	*equal_sign;

	equal_sign = ft_strchr(envp_entry, '=');
	if (equal_sign)
	{
		new_node->equal = 1;
		new_node->key = ft_substr(envp_entry, 0, equal_sign - envp_entry);
		if (!new_node->key)
			return (0);
		new_node->value = ft_strdup(equal_sign + 1);
		if (!new_node->value)
			return (0);
	}
	else
	{
		new_node->equal = 0;
		new_node->value = NULL;
		new_node->key = ft_strdup(envp_entry);
		if (!new_node->key)
			return (0);
	}
	return (1);
}

void	env_init(t_env **env_list, char **envp)
{
	int		i;
	t_env	*new_node;

	if (!env_list || !envp)
		return ;
	i = 0;
	*env_list = NULL;
	while (envp[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (cleanup_and_exit(env_list, NULL));
		init_new_node(new_node);
		if (!process_env_entry(new_node, envp[i]))
			return (cleanup_and_exit(env_list, new_node));
		env_add_back(env_list, new_node);
		i++;
	}
}
