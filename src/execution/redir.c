/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:00:42 by odana             #+#    #+#             */
/*   Updated: 2025/07/26 11:22:51 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	redir_in(t_redir *redir)
{
	int		fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd(redir->filename, STDERR_FILENO);
		ft_putendl_fd(": no such file or directory", STDERR_FILENO);
		exit(EXIT_GENERAL_ERROR);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	redir_out(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd(redir->filename, STDERR_FILENO);
		ft_putendl_fd(": permission denied", STDERR_FILENO);
		exit(EXIT_GENERAL_ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	redir_out_append(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd(redir->filename, STDERR_FILENO);
		ft_putendl_fd(": permission denied", STDERR_FILENO);
		exit(EXIT_GENERAL_ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/*
** setup_redir - Handles file redirections
**
** Supports: < (input), > (output), >> (append) and << LIM (heredoc)
*/
void	setup_redir(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*last_input;
	t_redir	*last_output;
	t_redir	*last_append;

	last_input = NULL;
	last_output = NULL;
	last_append = NULL;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			last_input = redir;
		else if (redir->type == REDIR_OUT)
			last_output = redir;
		else if (redir->type == REDIR_OUT_APPEND)
			last_append = redir;
		redir = redir->next;
	}
	if (last_input)
		redir_in(last_input);
	if (last_append)
		redir_out_append(last_append);
	else if (last_output)
		redir_out(last_output);
}
