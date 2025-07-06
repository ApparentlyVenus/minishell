/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:12:36 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 04:51:13 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

int	ft_exit(char **args, t_shell *shell)
{
	int		code;
	int		level;
	t_env	*shlvl;

	code = shell->exit_code;
	if (args[1])
		code = ft_atoi(args[1]);
	shlvl = get_env_value(shell->env, "SHLVL");
	if (shlvl && shlvl->value)
	{
		level = ft_atoi(shlvl->value);
		printf("Exiting shell level: %d\n", level);
	}
	free_env_list(shell->env);
	exit(code);
}
