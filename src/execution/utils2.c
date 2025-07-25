/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:57:23 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 07:14:34 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 *
 * Sets up the context structure with:
 * - Command count
 * - Pipe array
 * - Process ID array
 * - Environment variables
 */
t_exec	*setup_exec(t_node *cmd_list, t_env **env)
{
	t_exec	*ctx;

	if (!cmd_list)
		return (NULL);
	ctx = malloc(sizeof(t_exec));
	if (!ctx)
		return (NULL);
	ctx->cmd_count = count_commands(cmd_list);
	ctx->pipes = NULL;
	ctx->pids = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->pids)
		return (free(ctx), NULL);
	ctx->pipes = allocate_pipes(ctx->cmd_count);
	if (ctx->cmd_count > 1 && !ctx->pipes)
		return (free(ctx->pids), free(ctx), NULL);
	ctx->env = env;
	return (ctx);
}

/*
 * get_nth_command - Retrieves the nth command node from pipeline
 *
 * Navigates the left-associative AST to find a specific command
 * Used to iterate through commands in execution order
 */
t_node	*get_nth_command(t_node *node, int n)
{
	int	left_count;

	if (!node)
		return (NULL);
	if (node->type == NODE_PIPE)
	{
		left_count = count_commands(node->left);
		if (n < left_count)
			return (get_nth_command(node->left, n));
		else
			return (get_nth_command(node->right, n - left_count));
	}
	if (node->type == NODE_CMD && n == 0)
		return (node);
	return (NULL);
}

int	count_commands(t_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (count_commands(node->left) + count_commands(node->right));
	if (node->type == NODE_CMD)
		return (1);
	return (0);
}

void	parent_process(t_shell *shell, t_exec *ctx)
{
	close_pipes(ctx);
	signals_parent();
	shell->exit_code = wait_child(ctx);
	free_exec(ctx);
}
