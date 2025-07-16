/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_parsing_environment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:06:38 by yitani            #+#    #+#             */
/*   Updated: 2025/07/16 23:41:48 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	env_init(t_env **env_list, char **envp)
{
	int		i;
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
			new_node->key = strdup(envp[i]);
		}
		new_node->next = NULL;
		ft_lstadd_back(env_list, new_node);
	}
}

int	valid_entries_count(t_env *env)
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

char	**convert_env_to_array(t_env *env)
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
	while (i < count && curr)
	{
		if (curr->equal == 1)
		{
			temp[0] = ft_strjoin(curr->key, "=");
			temp[1] = ft_strjoin(temp[0], curr->value);
			envp[i] = temp[1];
			free(temp[0]);
			i++;
		}
		curr = curr->next;
	}
	envp[i] = NULL;
	return (envp);
}
