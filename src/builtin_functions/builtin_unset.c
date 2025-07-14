/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:04 by yitani            #+#    #+#             */
/*   Updated: 2025/07/14 08:50:50 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	unset_env_value(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	if (!env || !*env || !key || !is_valid_key(key))
		return (1);
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
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int	builtin_unset(char **args, t_env **env)
{
	int	failure;
	int	i;

	failure = 0;
	i = 0;
	while (args[i])
	{
		if (unset_env_value(env, args[i]) == 1)
			failure = 1;
		i++;
	}
	return (failure);
}
