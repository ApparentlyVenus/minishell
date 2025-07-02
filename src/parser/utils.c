/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:47:00 by odana             #+#    #+#             */
/*   Updated: 2025/07/02 17:46:26 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int add_arg_list(t_arg **list, char *value)
{
    t_arg *cur;
    t_arg *new;

    new = malloc(sizeof(t_arg));
    if (!new)
        return (0);
    new->value = ft_strdup(value);
    if (!new->value)
        return (free(new), 0);
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
        return (free_arg(arg_list), NULL);
    i = 0;
    current = arg_list;
    while (current)
    {
        argv[i++] = current->value;
        temp = current;
        current = current->next;
        free(temp);
    }
    argv[i] = NULL;
    return (argv);
}