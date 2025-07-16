/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:45 by yitani            #+#    #+#             */
/*   Updated: 2025/07/16 17:31:22 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_env(t_env **env)
{
	t_env	*current;

	current = *env;
	while (env)
	{
		if (current->equal == 1)
		{
			write(STDOUT_FILENO, *current->key, ft_strlen(current->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, *current->value, ft_strlen(current->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
}
