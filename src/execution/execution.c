/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:33:24 by yitani            #+#    #+#             */
/*   Updated: 2025/07/09 17:36:01 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** setup_exec - Initializes execution environment
**
** Sets up the context structure with:
** - Command count
** - Pipe array
** - Process ID array
** - Environment variables
*/
t_exec	*setup_exec(t_node *cmd_list, t_env *env)
{
	t_exec	*ctx;

	if (!cmd_list)
		return (NULL);
	ctx = malloc(sizeof(t_exec));
	if (!ctx)
		return (NULL);
	ctx->cmd_count = count_commands(cmd_list);
	ctx->pipes = allocate_pipes(ctx->cmd_count);
	if (ctx->cmd_count > 1 && !ctx->pipes)
		return (free(ctx->pids), free(ctx), NULL);
	ctx->pids = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->pids)
		return (free_pipes(ctx->pipes, ctx->cmd_count), free(ctx), NULL);
	ctx->exit_code = 0;
	ctx->env = env;
	return (ctx);
}

/*
** execute_external_command - Executes external programs
**
** Steps:
** - Check if command has '/' (absolute/relative path)
** - If not, search in PATH environment variable
** - Use execve to replace current process with command
*/
void	execute_external_command(t_node *cmd_node, t_exec *ctx)
{
	char	*cmd;
	char	*path;
	char	**envp;

	cmd = cmd_node->cmd->args[0];
	envp = convert_env_to_array(ctx->env);
	if (ft_strchr(cmd, '/'))
		path = cmd;
	else
		path = find_path(cmd, ctx->env);
	if (!path)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
		free_split(envp);
		exit(127);
	}
	execve(path, cmd_node->cmd->args, envp);
	perror("execve");
	if (path != cmd)
		free(path);
	free_split(envp);
	exit(127);
}

// ! yitani when you implement the builtins name them the way I named them
// ! or change the names directly in the execute_builtin function

/*
** execute_builtin - Executes built-in shell commands
**
** Steps:
** - Dispatches to appropriate builtin function based on command type
** - Called only in child processes for pipeline commands
** - Sets ctx->exit for proper exit code handling
*/
void	execute_builtin(t_node *cmd_node, t_exec *ctx)
{
	t_builtin	builtin_type;

	if (!cmd_node || !cmd_node->cmd || !cmd_node->cmd->args[0])
		return (ctx->exit_code = 1);
	builtin_type = get_builtin_type(cmd_node->cmd->args[0]);
	ctx->exit_code = 0;
	if (builtin_type == BUILTIN_CD)
		builtin_cd(ctx, cmd_node->cmd->args);
	else if (builtin_type == BUILTIN_ECHO)
		builtin_echo(cmd_node->cmd->args, ctx);
	else if (builtin_type == BUILTIN_ENV)
		builtin_env(ctx);
	else if (builtin_type == BUILTIN_EXIT)
		builtin_exit(cmd_node->cmd->args, ctx);
	else if (builtin_type == BUILTIN_EXPORT)
		ctx->exit_code = builtin_export(ctx, cmd_node->cmd->args);
	else if (builtin_type == BUILTIN_PWD)
		builtin_pwd(ctx);
	else if (builtin_type == BUILTIN_UNSET)
		builtin_unset(ctx, cmd_node->cmd->args);
	else
		ctx->exit_code = 1;
}

/*
** execute_single_command - Handles execution of one command
**
** Steps:
** - Checks if command is built-in or external
** - Sets up pipes and redirections
** - Executes the command appropriately
** - Only called in child processes
*/
void	execute_command(t_node *cmd_node, t_exec *ctx, int cmd_index)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_pipes(ctx, cmd_index);
	setup_redir(cmd_node->cmd);
	if (get_builtin_type(cmd_node->cmd->args[0]) != BUILTIN_NONE)
	{
		execute_builtin(cmd_node, ctx);
		exit(0);
	}
	else
	{
		execute_external_command(cmd_node, ctx);
		exit(127);
	}
}

/*
** execute_pipeline - Main execution function
**
** Process flow:
** 1. Set up execution context
** 2. Fork child process for each command
** 3. In child: set up pipes/redirections and execute
** 4. In parent: close pipes and wait for children
*/
void	execute_pipeline(t_node *cmd_list, t_env *env)
{
	t_exec	*ctx;
	t_node	*cmd_node;
	int		i;

	if (!cmd_list)
		return ;
	ctx = setup_exec(cmd_list, env);
	if (!ctx)
		return ;
	i = -1;
	while (++i < ctx->cmd_count)
	{
		cmd_node = get_nth_command(cmd_list, i);
		if (!cmd_node || !cmd_node->cmd)
			continue ;
		ctx->pids[i] = fork();
		if (ctx->pids[i] == -1)
		{
			kill_children(ctx, i);
			break ;
		}
		else if (ctx->pids[i] == 0)
			execute_command(cmd_node, ctx, i);
	}
	return (close_pipes(ctx), ctx->exit_code = wait_child(ctx), free_exec(ctx));
}
