/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 07:38:28 by odana             #+#    #+#             */
/*   Updated: 2025/07/11 08:14:41 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int handle_error(t_shell *shell, char *msg, int exit_code)
{
    if (!shell)
        return (1);
    
    // Set error message and exit code
    set_error(shell, msg);
    shell->exit_code = exit_code;
    
    // Print error message
    print_error(msg);
    
    // Check if this is a fatal error
    if (is_fatal_error(exit_code))
        return (handle_fatal_error(shell, msg, exit_code));
    
    // For non-fatal errors, continue shell execution
    return (0);
}

int handle_fatal_error(t_shell *shell, char *msg, int exit_code)
{
    if (!shell)
        exit(exit_code);
    
    // Set error state
    set_error(shell, msg);
    shell->exit_code = exit_code;
    
    // Print error and exit
    perror(msg);
    shell_exit(shell, exit_code);
    
    return (1); // Never reached, but for completeness
}

void set_error(t_shell *shell, char *error_msg)
{
    if (!shell)
        return;
    
    // Free previous error message
    if (shell->last_error)
    {
        free(shell->last_error);
        shell->last_error = NULL;
    }
    
    // Set new error message
    if (error_msg)
        shell->last_error = ft_strdup(error_msg);
}
