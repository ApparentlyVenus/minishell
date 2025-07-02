/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:33:54 by odana             #+#    #+#             */
/*   Updated: 2025/07/02 17:46:20 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node  *create_pipe_node(t_node *left, t_node *right)
{
    t_node  *pipe;
    
    pipe = (t_node *)malloc(sizeof(t_node));
    if (!pipe)
        return (free_node(left), free_node(right), NULL);
    pipe->type = NODE_PIPE;
    pipe->left = left;
    pipe->right = right;
    pipe->cmd = NULL;
    return (pipe);
}

t_node  *create_cmd_node(char **argv, t_redir *redir_list)
{
    t_cmd   *command;
    t_node  *node;

    command = (t_cmd *)malloc(sizeof(t_cmd));
    if (!command)
        return (free_split(argv), free_redir(redir_list), NULL);
    command->args = argv;
    command->redirs = redir_list;
    node = (t_node *)malloc(sizeof(t_node));
    if (!node)
        return (free_cmd(command), NULL);
    node->type = NODE_CMD;
    node->cmd = command;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

t_redir *create_redir_node(int type, char *filename)
{
    t_redir *redir;

    redir = (t_redir *)malloc(sizeof(t_redir));
    if (!redir)
        return (free(filename), NULL);
    redir->type = type;
    redir->filename = filename;
    redir->next = NULL;
    return (redir);
}