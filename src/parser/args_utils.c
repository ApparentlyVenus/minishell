/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:33:54 by odana             #+#    #+#             */
/*   Updated: 2025/06/29 06:55:54 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int add_arg_list(t_arg **list, char *value)
{
    t_arg *cur;
    t_arg *new;

    new = malloc(sizeof(t_arg));
    if (!new)
        return (0);
    new->value = ft_strdup(value);
    new->next = NULL;
    if (!*list)
        *list = new;
    else
    {
        cur = *list;
        while (cur->next)
            cur = cur->next;
        cur->next = new;
    }
    return (1);
}

char **process_args(t_arg *arg_list, int count)
{
    char    **argv;
    t_arg   *temp;
    t_arg   *current;
    int     i;

    argv = (char **)malloc(sizeof(char *) * (count + 1));
    if (!argv)
        return (NULL);
    i = 0;
    current = arg_list;
    while (temp)
    {
        argv[i++] = current->value;
        temp = current;
        current = current->next;
        free(temp);
    }
    argv[i] = NULL;
    return (argv);
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

t_redir *create_redir_node(int type, char *filename)
{
    t_redir *redir;

    redir = (t_redir *)malloc(sizeof(t_redir));
    if (!redir)
        return (NULL);
    redir->type = type;
    redir->filename = filename;
    redir->next = NULL;
    return (redir);
}