/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 08:53:08 by odana             #+#    #+#             */
/*   Updated: 2025/07/11 08:57:15 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Checks if the shell pointer is valid.
int is_valid_shell(t_shell *shell)
{
    return (shell != NULL);
}

// Prints the current state of the shell (for debugging).
void print_shell_state(t_shell *shell)
{
    if (!shell)
    {
        printf("Shell is NULL.\n");
        return;
    }
    printf("--- Minishell State ---\n");
    printf("Exit Code: %d\n", shell->exit_code);
    printf("Interactive: %s\n", shell->interactive ? "Yes" : "No");
    printf("Current Phase: ");
    switch (shell->current_phase)
    {
        case PHASE_NONE: printf("NONE\n"); break;
        case PHASE_TOKEN: printf("TOKENIZATION\n"); break;
        case PHASE_EXPAND: printf("EXPANSION\n"); break;
        case PHASE_PARSE: printf("PARSING (AST creation)\n"); break;
        case PHASE_EXECUTE: printf("EXECUTION\n"); break;
        default: printf("UNKNOWN\n"); break;
    }
    printf("-----------------------\n");
}