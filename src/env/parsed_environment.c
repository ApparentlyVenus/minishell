/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsed_environment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:06:38 by yitani            #+#    #+#             */
/*   Updated: 2025/07/02 21:39:40 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	return(envp);
}
