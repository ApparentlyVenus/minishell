/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:24:38 by odana             #+#    #+#             */
/*   Updated: 2025/07/16 17:30:22 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "minishell.h"

typedef enum e_phase
{
	PHASE_NONE,
	PHASE_TOKEN,
	PHASE_PARSE,
	PHASE_EXECUTE,
}	t_phase;

// Central shell struct
typedef struct s_shell
{
	t_env		*env;
	int			exit_code;
	int			interactive;
	t_token		*tokens;
	t_node		*ast;
	t_exec		*exec_ctx;
	char		*last_error;
	t_phase		current_phase;
}	t_shell;

// Core shell functions
t_shell	*shell_init(char **envp);
void	shell_cleanup(t_shell *shell);
void	shell_exit(t_shell *shell, int exit_code);
void	shell_reset_phase(t_shell *shell);

// Wrapper functions for main loop

int		shell_tokenize(t_shell *shell, char *input);
int		shell_expand(t_shell *shell);
int		shell_parse(t_shell *shell);
int		shell_execute(t_shell *shell);

// Cleanup functions
void	cleanup_tokens(t_shell *shell);
void	cleanup_ast(t_shell *shell);
void	cleanup_exec(t_shell *shell);

// Error handling
void	set_error(t_shell *shell, char *error_msg);
void	print_error(char *msg);
int		is_fatal_error(int exit_code);
int		handle_error(t_shell *shell, char *msg, int exit_code);

#endif