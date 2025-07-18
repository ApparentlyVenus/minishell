/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:45 by yitani            #+#    #+#             */
/*   Updated: 2025/07/18 15:44:49 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_env(t_env **env)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (current->equal == 1)
		{
			write(STDOUT_FILENO, current->key, ft_strlen(current->key));
			write(STDOUT_FILENO, "=", 1);
			if (!current->value)
				write(STDOUT_FILENO, "", 1);
			else
				write(STDOUT_FILENO, current->value, ft_strlen(current->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
}
