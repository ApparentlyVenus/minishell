/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:45 by yitani            #+#    #+#             */
/*   Updated: 2025/07/30 01:45:42 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	env_error(char **args)
{
	if (args[1] && args[2] && ft_strcmp(args[1], "-i") == 0)
	{
		printf("%s: initialization failed\n", args[2]);
		return (EXIT_GENERAL_ERROR);
	}
	printf("env: '%s': No such file or directory\n", args[1]);
	return (EXIT_GENERAL_ERROR);
}

int	builtin_env(t_env **env, char **args)
{
	t_env	*current;

	if (!env || !*env)
		return (EXIT_GENERAL_ERROR);
	if (args[1] || args[2])
		return (env_error(args));
	current = *env;
	while (current)
	{
		if (!current->key)
		{
			current = current->next;
			continue ;
		}
		if (current->equal == 1)
		{
			write(STDOUT_FILENO, current->key, ft_strlen(current->key));
			write(STDOUT_FILENO, "=", 1);
			if (current->value)
				write(STDOUT_FILENO, current->value, ft_strlen(current->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
