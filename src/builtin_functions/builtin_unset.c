/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:04 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 04:40:09 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	unset_env_value(t_env **env, char *key)
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

void	builtin_unset(t_shell *shell, t_token *arg)
{
	t_token	*current;
	int		failure;

	failure = 0;
	current = arg->next;
	while (current)
	{
		if (current && current->type == TOKEN_WORD
			&& unset_env_value(shell->env, current->value) == 1)
			failure = 1;
		current = current->next;
	}
	shell->exit_code = failure;
}
