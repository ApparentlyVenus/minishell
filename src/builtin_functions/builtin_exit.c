/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:12:36 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 07:52:54 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	size_of_arr(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		i++;
	}
	return (i);
}

static void	free_env_list(t_env **env)
{
	t_env	*tmp;

	while (*env)
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	builtin_exit(char **args, t_shell *shell)
{
	long long	code;

	printf("exit\n");
	if (size_of_arr(args) >= 2)
	{
		if (!ft_is_numeric(args[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n", args[1]);
			shell->exit_code = 2;
			free_env_list(shell->env);
			exit(shell->exit_code);
		}
		if (size_of_arr(args) > 2)
		{
			printf("minishell: exit: too many arguments\n");
			shell->exit_code = 1;
			return ;
		}
		code = ft_atoll(args[1]);
		shell->exit_code = (unsigned int)code;
	}
	free_env_list(shell->env);
	exit(shell->exit_code);
}
