/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:55:08 by odana             #+#    #+#             */
/*   Updated: 2025/07/13 19:05:06 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** setup_pipes - Configures pipe connections for a command
**
** Pipeline flow: a | b | c
** - a: stdout -> pipe[0][1]
** - b: stdin <- pipe[0][0], stdout -> pipe[1][1]
** - c: stdin <- pipe[1][0]
*/
void	setup_pipes(t_exec *ctx, int cmd_index)
{
	int	i;

	if (cmd_index > 0)
		dup2(ctx->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < ctx->cmd_count - 1)
		dup2(ctx->pipes[cmd_index][1], STDOUT_FILENO);
	i = 0;
	while (i < ctx->cmd_count - 1)
	{
		close(ctx->pipes[i][0]);
		close(ctx->pipes[i][1]);
		i++;
	}
}

/*
** allocate_pipes - Creates pipe pairs for inter-process communication
**
** For n commands, we need (n-1) pipes
** Each pipe has 2 file descriptors: [0] for reading, [1] for writing
** Example: "cmd1 | cmd2 | cmd3" needs 2 pipes
*/
int	**allocate_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	if (cmd_count < 2)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			free_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

/*
** get_nth_command - Retrieves the nth command node from pipeline
**
** Navigates the left-associative AST to find a specific command
** Used to iterate through commands in execution order
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

void	close_pipes(t_exec *ctx)
{
	int	i;

	if (!ctx->pipes)
		return ;
	i = 0;
	while (i < ctx->cmd_count - 1)
	{
		close(ctx->pipes[i][0]);
		close(ctx->pipes[i][1]);
		i++;
	}
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
