/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 07:38:28 by odana             #+#    #+#             */
/*   Updated: 2025/07/15 17:30:11 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void print_error(char *msg)
{
	if (!msg)
		return ;
	
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

int is_fatal_error(int exit_code)
{
	return (exit_code == EXIT_GENERAL_ERROR || exit_code == 1);
}

void set_error(t_shell *shell, char *error_msg)
{
	if (!shell)
		return ;
	if (shell->last_error)
	{
		free(shell->last_error);
		shell->last_error = NULL;
	}
	if (error_msg)
		shell->last_error = ft_strdup(error_msg);
}

int handle_error(t_shell *shell, char *msg, int exit_code)
{
	if (!shell)
		return (1);
	set_error(shell, msg);
	shell->exit_code = exit_code;
	print_error(msg);
	if (is_fatal_error(exit_code))
		shell_exit(shell, exit_code);
	return (0);
}
