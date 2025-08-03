/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:42:18 by odana             #+#    #+#             */
/*   Updated: 2025/08/01 14:40:44 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	execute_builtin_with_fork(t_node *node, t_shell *shell)
{
	t_builtin	type;
	char		**args;
	t_exec		ctx;
	pid_t		pid;
	int			status;

	type = get_builtin_type(node->cmd->args[0]->value);
	pid = fork();
	if (pid == 0)
	{
		signals_child();
		setup_redir(node->cmd);
		args = convert_args(node->cmd->args);
		if (!args)
			exit(EXIT_GENERAL_ERROR);
		ctx.env = &shell->env;
		ctx.cmd_count = 1;
		ctx.pipes = NULL;
		ctx.pids = NULL;
		exit(call_builtin_function(type, args, &ctx, shell));
	}
	else if (pid > 0)
	{
		signals_parent();
		waitpid(pid, &status, 0);
		signals_prompt();
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (EXIT_GENERAL_ERROR);
}

static int	execute_builtin_in_parent(t_node *node, t_shell *shell)
{
	t_builtin	type;
	char		**args;
	t_exec		ctx;
	int			exit_code;

	type = get_builtin_type(node->cmd->args[0]->value);
	args = convert_args(node->cmd->args);
	if (!args)
		return (EXIT_GENERAL_ERROR);
	ctx.env = &shell->env;
	ctx.cmd_count = 1;
	ctx.pipes = NULL;
	ctx.pids = NULL;
	exit_code = call_builtin_function(type, args, &ctx, shell);
	free_split(args);
	return (exit_code);
}

void	execute_parent_builtin(t_node *node, t_shell *shell)
{
	if (!node->cmd->args || !node->cmd->args[0] || 
		!node->cmd->args[0]->value || node->cmd->args[0]->value[0] == '\0')
	{
		shell->exit_code = EXIT_SUCCESS;
		return ;
	}
	expand_cmd(node->cmd, get_builtin_type(node->cmd->args[0]->value), shell);
	if (node->cmd->redirs)
		shell->exit_code = execute_builtin_with_fork(node, shell);
	else
		shell->exit_code = execute_builtin_in_parent(node, shell);
}

void	fork_single_command(t_exec *ctx, t_node *cmd_node, int i,
	t_shell *shell)
{
	if (is_empty_command(cmd_node->cmd->args))
	{
		ctx->pids[i] = fork();
		if (ctx->pids[i] == 0)
			exit(0);
		return ;
	}
	ctx->pids[i] = fork();
	if (ctx->pids[i] == -1)
	{
		kill_child(ctx, i);
		return ;
	}
	else if (ctx->pids[i] == 0)
		execute_command(cmd_node, ctx, i, shell);
}

void	execute_children_pipeline(t_node *node, t_shell *shell)
{
	t_exec	*ctx;
	t_node	*cmd_node;
	int		i;

	ctx = setup_exec(node, &shell->env);
	if (!ctx)
		return ;
	i = -1;
	while (++i < ctx->cmd_count)
	{
		cmd_node = get_nth_command(node, i);
		if (!cmd_node || !cmd_node->cmd)
			continue ;
		fork_single_command(ctx, cmd_node, i, shell);
		if (ctx->pids[i] == -1)
			break ;
	}
	parent_process(shell, ctx);
}

int	execute_parent_node(t_node *node, t_shell *shell)
{
	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == NODE_AND)
		return (execute_and(node, shell));
	else if (node->type == NODE_OR)
		return (execute_or(node, shell));
	else if (node->type == NODE_CMD)
	{
		execute_parent_builtin(node, shell);
		return (shell->exit_code);
	}
	return (EXIT_GENERAL_ERROR);
}

int	execute_children_node(t_node *node, t_shell *shell)
{
	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == NODE_AND)
		return (execute_and(node, shell));
	else if (node->type == NODE_OR)
		return (execute_or(node, shell));
	else
	{
		execute_children_pipeline(node, shell);
		return (shell->exit_code);
	}
}
