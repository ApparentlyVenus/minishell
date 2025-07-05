/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:00:42 by odana             #+#    #+#             */
/*   Updated: 2025/07/04 21:49:51 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	redir_in(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	redir_out(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	redir_out_append(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/*
** setup_redirections - Handles file redirections
**
** Supports: < (input), > (output), >> (append)
   TODO: Implement heredoc (<<)	
*/
void	setup_redir(t_cmd *cmd)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			redir_in(redir);
		else if (redir->type == REDIR_OUT)
			redir_out(redir);
		else if (redir->type == REDIR_OUT_APPEND)
			redir_out_append(redir);
		redir = redir->next;
	}
}
