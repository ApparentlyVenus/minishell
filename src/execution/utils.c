/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:52:31 by odana             #+#    #+#             */
/*   Updated: 2025/07/16 17:33:57 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*find_path(char *cmd, t_env *env_list)
{
	char	*env_path;
	char	**paths;
	char	*full_path;
	char	*part_path;
	int		i;

	env_path = get_env_value(env_list, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (full_path && access(full_path, X_OK) == 0)
			return (free_split(paths), full_path);
		free(full_path);
	}
	return (free_split(paths), NULL);
}

char	**convert_args(t_arg **args)
{
	int		count;
	char	**result;
	int		i;

	count = 0;
	if (!args)
		return (NULL);
	while (args[count])
		count++;
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		result[i] = ft_strdup(args[i]->value);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	wait_child(t_exec *ctx)
{
	int	status;
	int	exit_code;
	int	i;

	status = 0;
	exit_code = EXIT_SUCCESS;
	i = 0;
	while (i < ctx->cmd_count)
	{
		waitpid(ctx->pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				exit_code = EXIT_CTRL_C;
			else if (WTERMSIG(status) == SIGQUIT)
				exit_code = EXIT_CTRL_BACK;
			else
				exit_code = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (exit_code);
}

void	kill_child(t_exec *ctx, int i)
{	
	int	j;

	j = 0;
	if (ctx->pids[i] == -1)
		ft_putendl_fd("fork failed", STDERR_FILENO);
	while (j < i)
	{
		if (ctx->pids[j] > 0)
		{
			if (kill(ctx->pids[j], SIGTERM) == -1)
				ft_putendl_fd("kill failed", STDERR_FILENO);
		}
		j++;
	}
}

t_builtin	get_builtin_type(const char *cmd_name)
{
	if (!cmd_name)
		return (BUILTIN_NONE);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (BUILTIN_EXIT);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (BUILTIN_UNSET);
	return (BUILTIN_NONE);
}
