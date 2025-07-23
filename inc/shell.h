/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:24:38 by odana             #+#    #+#             */
/*   Updated: 2025/07/23 12:34:47 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "minishell.h"

// Core shell functions
t_shell	*shell_init(char **envp);
void	shell_cleanup(t_shell *shell);
void	shell_exit(t_shell *shell, int exit_code);
void	shell_reset_phase(t_shell *shell);

// Wrapper functions for main loop

int		shell_tokenize(t_shell *shell, char *input);
int		shell_parse(t_shell *shell);
int		shell_execute(t_shell *shell);

// signals

void	signals_prompt(void);
void	signals_child(void);
void	signals_parent(void);

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