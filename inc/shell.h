/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:24:38 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 22:11:43 by yitani           ###   ########.fr       */
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
void	signals_child_heredoc(void);

// Cleanup functions
void	cleanup_tokens(t_shell *shell);
void	cleanup_ast(t_shell *shell);
void	cleanup_exec(t_shell *shell);
void	free_env(t_env *env);

// Error handling
void	set_error(t_shell *shell, char *error_msg);
void	print_error(char *msg);
int		is_fatal_error(int exit_code);
int		handle_error(t_shell *shell, char *msg, int exit_code);

// Main functions + extras

int		main_loop(t_shell *shell);
int		process_input(t_shell *shell, char *input);
char	*get_prompt(t_env **env);
char	*get_input_line(t_env **env);
void	update_shlvl(t_shell *shell);
void	art(void);

// Set shell

t_shell	**get_shell_ref(void);
void	set_shell(t_shell *sh);
t_shell	*get_shell(void);

#endif