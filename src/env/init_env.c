/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:32:22 by yitani            #+#    #+#             */
/*   Updated: 2025/06/29 06:57:10 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			new_node->key = ft_substr(envp[i], 0, ft_strlen(envp[i]) - ft_strlen(new_node->value));
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
		if (ft_strcmp(env->key, key, ft_strlen(key)) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
