/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:15:17 by yitani            #+#    #+#             */
/*   Updated: 2025/07/05 22:13:41 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env_list(t_env *env)
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

int ft_exit(char **args, t_shell *shell)
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
