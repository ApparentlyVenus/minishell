/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:47:00 by odana             #+#    #+#             */
/*   Updated: 2025/06/29 00:58:11 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     is_redir(t_token *token)
{
    if (token->type == TOKEN_HERDOC
        || token->type == TOKEN_REDIR_IN
        || token->type == TOKEN_REDIR_OUT
        || token->type == TOKEN_REDIR_OUT_APPEND)
        return (1);
    return (0);
}

void    append_redir(t_redir **list, t_redir *new_redir)
{
    if (!new_redir)
        return ;
    new_redir->next = *list;
    *list = new_redir;
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
