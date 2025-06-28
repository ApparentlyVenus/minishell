/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:47:10 by odana             #+#    #+#             */
/*   Updated: 2025/06/28 22:38:42 by odana            ###   ########.fr       */
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

typedef struct s_command
{
    char    **args;
    t_redir *redirs;
}   t_command;

typedef enum e_node_type
{
    NODE_CMD,
    NODE_PIPE
}   t_node_type;

typedef struct s_node
{
    t_node_type     type;
    struct s_node   *left;
    struct s_node   *right;
}   t_node;

#endif