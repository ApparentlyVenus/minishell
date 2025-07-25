/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:29:06 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 13:29:50 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_shell	*shell_init(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = NULL;
	shell->exit_code = EXIT_SUCCESS;
	shell->interactive = isatty(STDIN_FILENO);
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->exec_ctx = NULL;
	shell->current_phase = PHASE_NONE;
	shell->last_error = NULL;
	env_init(&shell->env, envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	return (shell);
}

void	shell_cleanup(t_shell *shell)
{
	if (!shell)
		return ;
	cleanup_tokens(shell);
	cleanup_ast(shell);
	cleanup_exec(shell);
	if (shell->env)
		free_env(shell->env);
	if (shell->last_error)
		free(shell->last_error);
	free(shell);
}

void	shell_exit(t_shell *shell, int exit_code)
{
	if (shell)
	{
		shell->exit_code = exit_code;
		shell_cleanup(shell);
	}
	rl_clear_history();
	exit(exit_code);
}

void	shell_reset_phase(t_shell *shell)
{
	if (!shell)
		return ;
	cleanup_tokens(shell);
	cleanup_ast(shell);
	cleanup_exec(shell);
	shell->current_phase = PHASE_NONE;
	if (shell->last_error)
	{
		free(shell->last_error);
		shell->last_error = NULL;
	}
}

int	is_valid_shell(t_shell *shell)
{
	return (shell != NULL && shell->env != NULL);
}
