/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:28:12 by yitani            #+#    #+#             */
/*   Updated: 2025/07/28 22:40:04 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

// main execution function

void		execute_pipeline(t_shell *shell);

// helpers for pipeline execution

void		execute_command(t_node *cmd_node, t_exec *ctx, int i,
				t_shell *shell);
int			execute_builtin(t_node *cmd_node, t_exec *ctx, t_shell *shell);
int			call_builtin_function(t_builtin builtin_type, char **args,
				t_exec *ctx, t_shell *shell);
void		execute_external_command(t_exec *ctx,
				char **args);
void		parent_process(t_shell *shell, t_exec *ctx);
void		execute_parent_builtin(t_node *node, t_shell *shell);
void		execute_children_pipeline(t_node *node, t_shell *shell);
int			execute_parent_node(t_node *node, t_shell *shell);
int			execute_children_node(t_node *node, t_shell *shell);

// logical operator handeling

int			execute_and(t_node *node, t_shell *shell);
int			execute_or(t_node *node, t_shell *shell);

// redirection handeling

void		setup_redir(t_cmd *cmd);
int			open_redir_file(t_redir *redir);
void		handle_redir_error(t_redir *redir);
void		update_redir_fd(t_redir *redir, int new_fd,
				int *input_fd, int *output_fd);
void		apply_final_redir(int input_fd, int output_fd);

// setup pipes

void		setup_pipes(t_exec *ctx, int cmd_index);
int			**allocate_pipes(int cmd_count);
void		close_pipes(t_exec *ctx);

// frees

void		free_pipes(int **pipes, int cmd_count);
void		free_exec(t_exec *ctx);

// utils

t_exec		*setup_exec(t_node *cmd_list, t_env **env_list);
int			wait_child(t_exec *ctx);
void		kill_child(t_exec *ctx, int i);
t_node		*get_nth_command(t_node *node, int n);
t_builtin	get_builtin_type(char *cmd_name);
char		*find_path(char *cmd, t_env *env_list);
int			count_commands(t_node *node);
char		**convert_args(t_arg **args);
int			run_in_parent(t_node *ast);
char		**skip_empty_args(char **args);

#endif