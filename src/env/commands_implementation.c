/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_implementation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:04 by yitani            #+#    #+#             */
/*   Updated: 2025/07/03 05:24:02 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	get_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (0);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (1);
}

static t_token	*new_line_flag(t_token *args, int *i)
{
	*i = 0;
	while (args && ft_strcmp(args->value, "-n") == 0)
	{
		args = args->next;
		*i = 1;
	}
	return (args);
}

int	builtin_echo(t_token *args)
{
	int	i;

	if (args->next)
	{
		args = args->next;
		args = new_line_flag(args, &i);
		while (args)
		{
			if (args->type == TOKEN_WORD)
			{
				if (i == 1 && ft_strcmp(args->value, "\n") == 0)
				{
					args = args->next;
					continue ;
				}
				write(1, args->value, ft_strlen(args->value));
				if (args->next && args->next->type == TOKEN_WORD)
					write(1, " ", 1);
			}
			args = args->next;
		}
	}
	if (i == 0)
		write(1, "\n", 1);
	return (i);
}

void	builtin_env(t_env *env)
{
	while (env)
	{
		if (env->equal == 1)
		{
			write(1, env->key, ft_strlen(env->key));
			write(1, "=", 1);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\n", 1);
		}
		env = env->next;
	}
}

void	builtin_unset(t_env **env, t_token *arg)
{
	t_token	*current;

	current = arg->next;
	while (current)
	{
		if (current && current->type == TOKEN_WORD)
			unset_env_value(env, current->value);
		current = current->next;
	}
}
