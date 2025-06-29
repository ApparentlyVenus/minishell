/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:54:08 by odana             #+#    #+#             */
/*   Updated: 2025/06/29 09:15:40 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  
**  call pipeline, if there are trailing tokens free and return NULL
*/

t_node  *parse_input(t_token **tokens)
{
    t_node  *ast;
    
    if (!tokens || !*tokens)
        return (NULL);
    ast = parse_pipeline(tokens);
    if (!ast)
        return (NULL);
    if (*tokens)
    {
        free_node(ast);
        return (NULL);
    }
    return (ast);
}

/*
**  parse left side, if there are extra tokens start parsing right side.
**  if there is a right side, create a pipeline node with both left and right.
**  if no extra tokens, return left as is.
*/

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
            return (free_node(left), NULL);
        return (create_pipe_node(left, right));
    }
    return (left);
}

/*
**  we need to build both a list of arguments and a list of redirections,
**  eventually we are going to convert the list of arguments to a double array
**  using the process_args function
**
**  first: parse words into arguments 
**  
*/

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
            return (free_arg(arg_list), NULL);
        count++;
        *tokens = (*tokens)->next;
    }
    while (*tokens && is_redir(*tokens))
    {
        redir = parse_redir(tokens);
        if (!redir)
            return (free_arg(arg_list), free_redir(redir_list), (NULL));
        append_redir(&redir_list, redir);
    }
    if (count == 0 && !redir_list)
        return (NULL);
    return (create_cmd_node(process_args(arg_list, count), redir_list));
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
    filename = ft_strdup((*tokens)->value);
    *tokens = (*tokens)->next;
    return (create_redir_node(type, filename));
}
