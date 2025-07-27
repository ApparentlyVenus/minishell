/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:47:10 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 00:29:31 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// main parsing function

t_node	*parse_input(t_token **tokens, t_env *env);

// parsing helpers

t_node	*parse_pipeline(t_token **tokens, t_env *env, int min_priority);
t_node	*parse_command(t_token **tokens, t_env *env);
t_redir	*parse_redir(t_token **tokens, t_env *env);
t_node	*parse_group(t_token **tokens, t_env *env);
t_node	*parse_command_or_group(t_token **tokens, t_env *env);

// node creation

t_node	*create_pipe_node(t_node *left, t_node *right);
t_node	*create_and_node(t_node *left, t_node *right);
t_node	*create_or_node(t_node *left, t_node *right);
t_node	*create_cmd_node(t_arg **args, t_redir *redir_list);
t_redir	*create_redir_node(int type, char *filename);

// heredoc
t_redir	*process_heredoc(char *delimiter, t_env *env, int single, int doubl);
char	*unquote_delimiter(char *delimiter);
int		is_delimiter_quoted(char *delimiter);
char	*create_temp_file(char *content);
char	*collect_heredoc_content(char *delimiter, int expand, t_env *env);
char	*append_heredoc_line(char *content, char *line);
char	*get_suffix(void);
void	cleanup_heredoc_files(t_cmd *cmd);

// heredoc child

void	heredoc_child_process(t_heredoc_data *data);
char	*read_filename_from_child(int pipe_fd[2]);

// utils

int		is_redir(t_token *token);
void	append_redir(t_redir **list, t_redir *new_redir);
t_arg	**process_args(t_arg *arg_list, int count);
int		add_arg_list(t_arg **list, t_token *token);
int		get_token_priority(t_token *tok);
t_node	*handle_operator(t_node *left, t_token **tokens,
			t_env *env, t_op_info op);

// frees

void	free_cmd(t_cmd *cmd);
void	free_redir(t_redir *redir);
void	free_node(t_node *node);
void	free_arg(t_arg *arg_list);
#endif