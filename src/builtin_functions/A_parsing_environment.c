/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_parsing_environment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:06:38 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 01:48:44 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

static int	process_env_node(char **envp, t_env *curr, int i)
{
	char	*temp_key;

	if (curr->equal != 1)
		return (-1);
	temp_key = ft_strjoin(curr->key, "=");
	if (!temp_key)
		return (0);
	if (curr->value == NULL)
		envp[i] = ft_strdup(temp_key);
	else
		envp[i] = ft_strjoin(temp_key, curr->value);
	free(temp_key);
	if (!envp[i])
		return (0);
	return (1);
}

char	**convert_env_to_array(t_env *env)
{
	int		count;
	char	**envp;
	int		i;
	int		result;
	t_env	*curr;

	count = valid_entries_count(env);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	curr = env;
	while (curr && i < count)
	{
		result = process_env_node(envp, curr, i);
		if (result == 0)
			return (cleanup_envp(envp, i), NULL);
		if (result == 1)
			i++;
		curr = curr->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	**cleanup_envp(char **envp, int count)
{
	while (--count >= 0)
		free(envp[count]);
	free(envp);
	return (NULL);
}
