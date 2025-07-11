/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:31:58 by odana             #+#    #+#             */
/*   Updated: 2025/07/11 08:14:39 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void cleanup_tokens(t_shell *shell)
{
    if (shell && shell->tokens)
    {
        free_tokens(&shell->tokens);
        shell->tokens = NULL;
    }
}

void cleanup_expanded_tokens(t_shell *shell)
{
    if (shell && shell->expanded_tokens)
    {
        free_tokens(&shell->expanded_tokens);
        shell->expanded_tokens = NULL;
    }
}

void cleanup_ast(t_shell *shell)
{
    if (shell && shell->ast)
    {
        free_node(shell->ast);
        shell->ast = NULL;
    }
}

void cleanup_exec(t_shell *shell)
{
    if (shell && shell->exec_ctx)
    {
        free_exec(shell->exec_ctx);
        shell->exec_ctx = NULL;
    }
}

