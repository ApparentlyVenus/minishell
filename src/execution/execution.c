/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:33:24 by yitani            #+#    #+#             */
/*   Updated: 2025/07/28 21:50:14 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		return (builtin_pwd(ctx->env));
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
	expand_all_args(cmd_node->cmd, type, shell);
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

void	execute_pipeline(t_shell *shell)
{
	if (!shell->ast)
		return ;
	if (run_in_parent(shell->ast))
		execute_parent_node(shell->ast, shell);
	else
		execute_children_node(shell->ast, shell);
}
