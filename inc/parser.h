/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:47:10 by odana             #+#    #+#             */
/*   Updated: 2025/07/02 17:50:12 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef enum e_redir_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_OUT_APPEND,
    HERE_DOC
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type    type;
    char            *filename;
    struct s_redir  *next;
}   t_redir;

typedef struct s_arg
{
    char            *value;
    struct s_arg    *next;
}   t_arg;

typedef struct s_cmd
{
    char    **args;
    t_redir *redirs;
}   t_cmd;

typedef enum e_node_type
{
    NODE_CMD,
    NODE_PIPE,
    NODE_SEQUENCE
}   t_node_type;

typedef struct s_node
{
    t_node_type     type;
    struct s_cmd    *cmd;
    struct s_node   *left;
    struct s_node   *right;
}   t_node;


// main parsing functions

t_node  *parse_input(t_token **tokens);

// parsing helpers

t_node  *parse_pipeline(t_token **tokens);
t_node  *parse_command(t_token **tokens);
t_redir *parse_redir(t_token **tokens);
t_node  *create_pipe_node(t_node *left, t_node *right);
t_node  *create_cmd_node(char **argv, t_redir *redir_list);
t_redir *create_redir_node(int type, char *filename);

// utils

int     is_redir(t_token *token);
void    append_redir(t_redir **list, t_redir *new_redir);
int     add_arg_list(t_arg **list, char *value);
char    **process_args(t_arg *arg_list, int count);

// frees

void	free_split(char **args);
void	free_cmd(t_cmd *cmd);
void	free_redir(t_redir *redir);
void	free_node(t_node *node);
void    free_arg(t_arg *arg_list);
#endif