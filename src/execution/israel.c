/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   israel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:42:18 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 07:13:19 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	kill_child(t_exec *ctx, int i)
{
	int	j;

	j = 0;
	if (ctx->pids[i] == -1)
		ft_putendl_fd("fork failed", STDERR_FILENO);
	while (j < i)
	{
		if (ctx->pids[j] > 0)
		{
			if (kill(ctx->pids[j], SIGTERM) == -1)
				ft_putendl_fd("kill failed", STDERR_FILENO);
		}
		j++;
	}
}

void	execute_children(t_shell *shell)
{
	t_exec	*ctx;
	t_node	*cmd_node;
	int		i;

	ctx = setup_exec(shell->ast, &shell->env);
	if (!ctx)
		return ;
	i = -1;
	while (++i < ctx->cmd_count)
	{
		cmd_node = get_nth_command(shell->ast, i);
		if (!cmd_node || !cmd_node->cmd)
			continue ;
		ctx->pids[i] = fork();
		if (ctx->pids[i] == -1)
		{
			kill_child(ctx, i);
			break ;
		}
		else if (ctx->pids[i] == 0)
			execute_command(cmd_node, ctx, i, shell);
	}
	parent_process(shell, ctx);
}

void	execute_parent(t_shell *shell)
{
	t_builtin	type;
	char		**args;

	printf("DEBUG: execute_parent called\n");
	type = get_builtin_type(shell->ast->cmd->args[0]->value);
	printf("DEBUG: builtin type = %d\n", type);
	expand_cmd(shell->ast->cmd, type, shell);
	printf("DEBUG: expand_cmd completed\n");
	args = convert_args(shell->ast->cmd->args);
	if (!args)
	{
		printf("DEBUG: convert_args failed\n");
		shell->exit_code = EXIT_GENERAL_ERROR;
		return ;
	}
	printf("DEBUG: about to call builtin function\n");
	shell->exit_code = call_builtin_function(type, args, NULL, shell);
	printf("DEBUG: builtin function returned %d\n", shell->exit_code);
	free_split(args);
	printf("DEBUG: execute_parent completed\n");
}
