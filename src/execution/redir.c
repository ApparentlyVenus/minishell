/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:00:42 by odana             #+#    #+#             */
/*   Updated: 2025/07/15 14:46:49 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	redir_in(t_redir *redir)
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

void	redir_out(t_redir *redir)
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

void	redir_out_append(t_redir *redir)
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

void redir_heredoc(t_redir *redir)
{
    int pipefd[2];
    char *line;
    
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, redir->filename) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
}

/*
** setup_redir - Handles file redirections
**
** Supports: < (input), > (output), >> (append) and << LIM (heredoc)
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
		else if (redir->type == HERE_DOC)
			redir_heredoc(redir);
		redir = redir->next;
	}
}
