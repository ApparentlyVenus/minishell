/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:40:27 by odana             #+#    #+#             */
/*   Updated: 2025/07/04 20:53:00 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_pipes(int **pipes, int cmd_count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipes[i])
			free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	free_exec(t_exec *ctx)
{
	if (!ctx)
		return ;
	if (ctx->pids)
		free(ctx->pids);
	free_pipes(ctx->pipes, ctx->cmd_count);
	free(ctx);
}

void	free_split(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}
