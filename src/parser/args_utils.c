/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:33:54 by odana             #+#    #+#             */
/*   Updated: 2025/06/29 00:46:58 by odana            ###   ########.fr       */
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
