/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:54:08 by odana             #+#    #+#             */
/*   Updated: 2025/06/29 06:48:53 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node  *parse_pipeline(t_token **tokens)
{
    t_node  *left;
    t_node  *right;
    t_token *current;

    if (!tokens || !*tokens)
        return (NULL);
    left = parse_command(tokens);
    if (!left)
        return (NULL);
    current = *tokens;
    if (current && current->type == TOKEN_PIPE)
    {
        *tokens = current->next;
        right = parse_pipeline(tokens);
        if (!right)
            return (NULL);
        return (create_pipe_node(left, right));
    }
    return (left);
}

t_node  *create_pipe_node(t_node *left, t_node *right)
{
    t_node  *pipe;
    
    pipe = (t_node *)malloc(sizeof(t_node));
    if (!pipe)
        return (NULL);
    pipe->type = NODE_PIPE;
    pipe->left = left;
    pipe->right = right;
    pipe->cmd = NULL;
    return (pipe);
}

t_node  *parse_command(t_token **tokens)
{
    int     count;
    t_arg   *arg_list;
    t_redir *redir_list;
    t_redir *redir;
    
    count = 0;
    arg_list = NULL;
    redir_list = NULL;
    while (*tokens && (*tokens)->type == TOKEN_WORD)
    {
        if (!add_arg_list(&arg_list, (*tokens)->value));
            return (NULL);
        count++;
        *tokens = (*tokens)->next;
    }
    while (*tokens && is_redir(*tokens))
    {
        redir = parse_redir(tokens);
        if (!redir)
            return (NULL);
        append_redir(&redir_list, redir);
    }
    if (count == 0 && !redir_list)
        return (NULL);
    return (create_cmd_node(process_args(arg_list, count), redir_list));
}

t_node  *create_cmd_node(char **argv, t_redir *redir_list)
{
    t_cmd   *command;
    t_node  *node;

    command = (t_cmd *)malloc(sizeof(t_cmd));
    if (!command)
        return (NULL);
    command->args = argv;
    command->redirs = redir_list;
    node = (t_node *)malloc(sizeof(t_node));
    if (!node)
        return (NULL);
    node->type = NODE_CMD;
    node->cmd = command;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

t_redir *parse_redir(t_token **tokens)
{
    int     type;
    char    *filename;
    
    if (!*tokens)
        return (NULL);
    type = (*tokens)->type;
    *tokens = (*tokens)->next;
    if (!*tokens || (*tokens)->type != TOKEN_WORD)
        return (NULL);
    filename = (*tokens)->value;
    *tokens = (*tokens)->next;
    return (create_redir_node(type, filename));
}
