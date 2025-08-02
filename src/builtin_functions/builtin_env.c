/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:45 by yitani            #+#    #+#             */
/*   Updated: 2025/08/02 20:21:20 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
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
				write(STDOUT_FILENO, current->value,
					ft_strlen(current->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		current = current->next;
	}
}

static int	find_command_start(char **args)
{
	int	i;

	i = 1;
	if (args[1] && ft_strcmp(args[1], "-i") == 0)
		i = 2;
	while (args[i] && ft_strchr(args[i], '='))
		i++;
	if (!args[i])
		return (-1);
	return (i);
}

static int	handle_no_command(char **args, t_env *env)
{
	if (args[1] && ft_strcmp(args[1], "-i") == 0)
		return (EXIT_SUCCESS);
	print_env(env);
	return (EXIT_SUCCESS);
}

int	builtin_env(t_env **env, char **args)
{
	t_env	*working_env;
	int		cmd_idx;
	int		start_idx;
	int		result;

	if (!env || !*env)
		return (EXIT_GENERAL_ERROR);
	if (!args || !args[1])
		return (print_env(*env), EXIT_SUCCESS);
	cmd_idx = find_command_start(args);
	if (cmd_idx == -1)
		return (handle_no_command(args, *env));
	working_env = env_copy_or_clean(args, *env);
	if (!working_env && args[1] && ft_strcmp(args[1], "-i") != 0)
		return (EXIT_GENERAL_ERROR);
	if (args[1] && ft_strcmp(args[1], "-i") == 0)
		start_idx = 2;
	else
		start_idx = 1;
	result = env_apply_changes(&working_env, args, start_idx, cmd_idx);
	if (result != EXIT_SUCCESS)
		return (free_env(working_env), result);
	result = env_execute_command(args + cmd_idx, working_env);
	return (free_env(working_env), result);
}
