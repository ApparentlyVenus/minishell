/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:55:08 by odana             #+#    #+#             */
/*   Updated: 2025/07/04 21:51:18 by odana            ###   ########.fr       */
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
