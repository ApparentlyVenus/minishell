/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_parsing_environment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:06:38 by yitani            #+#    #+#             */
/*   Updated: 2025/07/25 01:06:12 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	env_init(t_env **env_list, char **envp)
{
	int		i;
	t_env	*new_node;
	char	*equal_sign;

	if (!env_list || !envp)
		return ;
	i = 0;
	*env_list = NULL;
	while (envp[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
		{
			free_env(*env_list);
			*env_list = NULL;
			return ;
		}
		new_node->key = NULL;
		new_node->value = NULL;
		new_node->equal = 0;
		new_node->next = NULL;
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
			new_node->key = ft_strdup(envp[i]);
		}
		if (!new_node->key || (equal_sign && !new_node->value))
		{
			if (new_node->key)
				free(new_node->key);
			if (new_node->value)
				free(new_node->value);
			free(new_node);
			free_env(*env_list);
			*env_list = NULL;
			return ;
		}
		env_add_back(env_list, new_node);
		i++;
	}
}

void	env_add_back(t_env **env_list, t_env *new_node)
{
	t_env	*last;

	if (!env_list || !new_node)
		return ;
	if (*env_list == NULL)
	{
		*env_list = new_node;
		return ;
	}
	last = *env_list;
	while (last->next)
		last = last->next;
	last->next = new_node;
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
