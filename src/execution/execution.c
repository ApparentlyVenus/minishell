/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:33:24 by yitani            #+#    #+#             */
/*   Updated: 2025/07/25 07:47:41 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * execute_external_command - Executes external programs
 *
 * Steps:
 * - Check if command has '/' (absolute/relative path)
 * - If not, search in PATH environment variable
 * - Use execve to replace current process with command
 */
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
	execve(path, args, envp);
	ft_putendl_fd("execve failed", STDERR_FILENO);
	if (path != cmd)
		free(path);
	free_split(envp);
	exit(127);
}

// ! yitani when you implement the builtins name them the way I named them
// ! or change the names directly in the execute_builtin function

/*
 * execute_builtin - Executes built-in shell commands
 *
 * Steps:
 * - Dispatches to appropriate builtin function based on command type
 * - Sets ctx->exit for proper exit code handling
 * - Called only in child processes for pipeline commands
 */
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
	{
		builtin_exit(args, shell);
		return (EXIT_SUCCESS);
	}
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

/*
 * execute_single_command - Handles execution of one command
 *
 * Steps:
 * - Checks if command is built-in or external
 * - Sets up pipes and redirections
 * - Executes the command appropriately
 * - Only called in child processes
 */
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
		execute_external_command(ctx, args);
		free_split(args);
		exit(127);
	}
}

/*
 * execute_pipeline - Main execution function
 *
 * Process flow:
 * 1. Set up execution context
 * 2. Fork child process for each command
 * 3. In child: set up pipes/redirections and execute
 * 4. In parent: close pipes and wait for children
 */
void	execute_pipeline(t_shell *shell)
{
	if (!shell->ast)
		return ;
	if (run_in_parent(shell->ast))
		execute_parent_node(shell->ast, shell);
	else
		execute_children_node(shell->ast, shell);
}
