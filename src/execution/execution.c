/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:33:24 by yitani            #+#    #+#             */
/*   Updated: 2025/07/26 19:16:28 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	if_directory_or_invalid(char *path, char *cmd, char **envp)
{
	struct stat	sb;

	if (stat(path, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
			handle_directory_error(path, cmd, envp);
		if (access(path, X_OK) != 0)
			handle_permission_error(path, cmd, envp);
	}
	else
		handle_not_found_error(path, cmd, envp);
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

int	call_builtin_function(t_builtin builtin_type, char **args, t_exec *ctx,
	t_shell *shell)
{
	if (builtin_type == BUILTIN_CD)
		return (builtin_cd(args + 1, ctx->env));
	else if (builtin_type == BUILTIN_ECHO)
		return (builtin_echo(args + 1));
	else if (builtin_type == BUILTIN_ENV)
	{
		builtin_env(ctx->env);
		return (EXIT_SUCCESS);
	}
	else if (builtin_type == BUILTIN_EXIT)
		return (builtin_exit(args, shell));
	else if (builtin_type == BUILTIN_EXPORT)
		return (builtin_export(args + 1, ctx->env));
	else if (builtin_type == BUILTIN_PWD)
		return (builtin_pwd());
	else if (builtin_type == BUILTIN_UNSET)
		return (builtin_unset(args + 1, ctx->env));
	else
		return (EXIT_GENERAL_ERROR);
}

int	execute_builtin(t_node *cmd_node, t_exec *ctx, t_shell *shell)
{
	t_builtin	builtin_type;
	char		**args;
	int			exit_code;

	if (!cmd_node || !cmd_node->cmd || !cmd_node->cmd->args[0])
		return (EXIT_GENERAL_ERROR);
	builtin_type = get_builtin_type(cmd_node->cmd->args[0]->value);
	args = convert_args(cmd_node->cmd->args);
	if (!args)
		return (EXIT_GENERAL_ERROR);
	exit_code = call_builtin_function(builtin_type, args, ctx, shell);
	free_split(args);
	return (exit_code);
}

void	execute_command(t_node *cmd_node, t_exec *ctx, int i, t_shell *shell)
{
	t_builtin	type;
	char		**args;
	int			exit_code;

	signals_child();
	setup_pipes(ctx, i);
	if (!cmd_node->cmd->args || !cmd_node->cmd->args[0])
		exit(EXIT_GENERAL_ERROR);
	type = get_builtin_type(cmd_node->cmd->args[0]->value);
	expand_cmd(cmd_node->cmd, type, shell);
	setup_redir(cmd_node->cmd);
	if (type != BUILTIN_NONE)
	{
		exit_code = execute_builtin(cmd_node, ctx, shell);
		exit(exit_code);
	}
	else
	{
		args = convert_args(cmd_node->cmd->args);
		args = skip_empty_args(args);
		if (!args[0])
			exit(0);
		execute_external_command(ctx, args);
		return (free_split(args), exit(127));
	}
}
