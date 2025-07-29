/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:31:05 by odana             #+#    #+#             */
/*   Updated: 2025/07/30 01:55:25 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	print_something(char *cmd, char **envp)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	free_split(envp);
	exit(127);
}

void	if_directory_or_invalid(char *path, char *cmd, char **envp)
{
	struct stat		sb;

	if (stat(path, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Is a directory", STDERR_FILENO);
			if (path != cmd)
				free(path);
			free_split(envp);
			exit(126);
		}
		if (access(path, X_OK) != 0)
		{
			perror(cmd);
			if (path != cmd)
				free(path);
			free_split(envp);
			exit(126);
		}
	}
	else
		print_something(cmd, envp);
}

void	execute_external_command(t_exec *ctx, char **args)
{
	char	*cmd;
	char	*path;
	char	**envp;

	cmd = args[0];
	envp = convert_env_to_array(*ctx->env);
	if (ft_strchr(cmd, '/'))
		path = cmd;
	else
		path = find_path(cmd, *ctx->env);
	if (!path)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		free_split(envp);
		exit(127);
	}
	if_directory_or_invalid(path, cmd, envp);
	execve(path, args, envp);
	ft_putendl_fd("execve failed", STDERR_FILENO);
	if (path != cmd)
		free(path);
	free_split(envp);
	exit(127);
}

char	**skip_empty_args(char **args)
{
	while (args[0] && args[0][0] == '\0')
		args++;
	return (args);
}
