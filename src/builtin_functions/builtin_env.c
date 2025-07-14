/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:45 by yitani            #+#    #+#             */
/*   Updated: 2025/07/14 08:49:31 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void builtin_env(t_env **env)
{
	t_env	*current;

	current = *env;
	while (env)
	{
		if (current->equal == 1)
		{
			write(1, *current->key, ft_strlen(current->key));
			write(1, "=", 1);
			write(1, *current->value, ft_strlen(current->value));
			write(1, "\n", 1);
		}
		current = current->next;
	}
}
