/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:28:12 by yitani            #+#    #+#             */
/*   Updated: 2025/07/05 13:05:23 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include "parser.h"
# include "env.h"

// enum for builtin command identification
typedef enum e_builtin
{
	BUILTIN_NONE,
	BUILTIN_CD,
	BUILTIN_ECHO,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	BUILTIN_EXPORT,
	BUILTIN_PWD,
	BUILTIN_UNSET
}	t_builtin;

// execution context for a pipeline
typedef struct s_exec
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	int		exit;
	t_env	*env_list;
}	t_exec;

// main execution function

void		execute_pipeline(t_node *cmd_list, t_env *env_list);

// helpers for pipeline execution

void		execute_command(t_node *cmd_node, t_exec *ctx, int cmd_index);
void		execute_builtin(t_node *cmd_node, t_exec *ctx);
void		execute_external_command(t_node *cmd_node, t_exec *ctx);
t_exec		*setup_exec(t_node *cmd_list, t_env *env_list);

// redirection handeling

int			setup_redir(t_cmd *cmd);
void		redir_in(t_redir *redir);
void		redir_out(t_redir *redir);
void		redir_out_append(t_redir *redir);
// TODO:	redir_heredoc();

// setup pipes

int			setup_pipes(t_exec *ctx, int cmd_index);
int			**allocate_pipes(int cmd_count);
void		close_pipes(t_exec *ctx);

// frees

void		free_pipes(int **pipes, int cmd_count);
void		free_exec(t_exec *ctx);
void		free_split(char **args);

// utils
int			wait_for_children(t_exec *ctx);
t_node		*get_nth_command(t_node *node, int n);
t_builtin	get_builtin_type(const char *cmd_name);
char		*find_path(char *cmd, t_env *env_list);
int			count_commands(t_node *node);

#endif