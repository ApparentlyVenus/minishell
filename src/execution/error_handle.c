/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:15:56 by odana             #+#    #+#             */
/*   Updated: 2025/07/26 19:16:10 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_directory_error(char *path, char *cmd, char **envp)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": Is a directory", STDERR_FILENO);
	cleanup_and_exit(path, cmd, envp, 126);
}

void	handle_permission_error(char *path, char *cmd, char **envp)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": Permission denied", STDERR_FILENO);
	cleanup_and_exit(path, cmd, envp, 126);
}

void	handle_not_found_error(char *path, char *cmd, char **envp)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	cleanup_and_exit(path, cmd, envp, 127);
}
