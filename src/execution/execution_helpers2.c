/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 21:31:05 by odana             #+#    #+#             */
/*   Updated: 2025/08/03 13:41:05 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static	void	print_something(char *cmd, char **envp, char **args)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	free_split(envp);
	free_split(args);
	exit(127);
}

void	if_directory_or_invalid(char *path, char *cmd, char **envp,
		char **args)
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
			return (free_split(envp), free_split(args), exit(126));
		}
		if (access(path, X_OK) != 0)
		{
			perror(cmd);
			if (path != cmd)
				free(path);
			free_split(envp);
			free_split(args);
			exit(126);
		}
	}
	else
		print_something(cmd, envp, args);
}

void	execute_external_command(t_exec *ctx, char **args)
{
	char	*cmd;
	char	*path;
	char	**envp;

	if (!args || !args[0] || args[0][0] == '\0')
	{
		if (args)
			free_split(args);
		exit(127);
	}
	cmd = args[0];
	envp = convert_env_to_array(*ctx->env);
	if (ft_strchr(cmd, '/'))
		path = cmd;
	else
		path = find_path(cmd, *ctx->env);
	if (!path)
		return (ft_putstr_fd(cmd, 2), ft_putendl_fd(": command not found",
				STDERR_FILENO), free_split(envp), exit(127));
	if_directory_or_invalid(path, cmd, envp, args);
	execve(path, args, envp);
	ft_putendl_fd("execve failed", STDERR_FILENO);
	if (path != cmd)
		free(path);
	return (free_split(envp), free_split(args), exit(127));
}

char	**skip_empty_args(char **args, t_arg **original_args)
{
	int	i;
	int	j;

	if (!args || !original_args)
		return (args);
	
	i = 0;
	j = 0;
	
	while (args[i] && args[i][0] == '\0')
	{
		if (original_args[j] && 
			!(original_args[j]->single_quotes || original_args[j]->double_quotes))
		{
			i++;
			j++;
		}
		else
			break;
	}
	return (&args[i]);
}

