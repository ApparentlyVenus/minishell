/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:14:07 by yitani            #+#    #+#             */
/*   Updated: 2025/08/02 21:38:50 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	env_execute_builtin(char **cmd_args, t_env *env)
{
	t_builtin	builtin_type;

	builtin_type = get_builtin_type(cmd_args[0]);
	if (builtin_type == BUILTIN_NONE)
		return (-1);
	if (builtin_type == BUILTIN_CD)
		return (builtin_cd(cmd_args + 1, &env));
	else if (builtin_type == BUILTIN_ECHO)
		return (builtin_echo(cmd_args + 1));
	else if (builtin_type == BUILTIN_ENV)
		return (builtin_env(&env, cmd_args));
	else if (builtin_type == BUILTIN_EXPORT)
		return (builtin_export(cmd_args + 1, &env));
	else if (builtin_type == BUILTIN_PWD)
		return (builtin_pwd(&env, cmd_args));
	else if (builtin_type == BUILTIN_UNSET)
		return (builtin_unset(cmd_args + 1, &env));
	return (-1);
}

int	env_execute_command(char **cmd_args, t_env *env)
{
	t_exec	ctx;
	pid_t	pid;
	int		status;
	int		builtin_result;

	builtin_result = env_execute_builtin(cmd_args, env);
	if (builtin_result != -1)
		return (builtin_result);
	ctx.env = &env;
	ctx.cmd_count = 1;
	ctx.pipes = NULL;
	ctx.pids = NULL;
	pid = fork();
	if (pid == 0)
	{
		signals_child();
		return (execute_external_command(&ctx, cmd_args), exit(127), 127);
	}
	else if (pid > 0)
	{
		signals_parent();
		return (waitpid(pid, &status, 0), (WEXITSTATUS(status)));
	}
	else
		return (EXIT_GENERAL_ERROR);
}
