/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:00:42 by odana             #+#    #+#             */
/*   Updated: 2025/07/27 12:57:30 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	open_redir_file(t_redir *redir)
{
	int		fd;
	int		flags;
	mode_t	mode;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_OUT)
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		mode = 0644;
		fd = open(redir->filename, flags, mode);
	}
	else if (redir->type == REDIR_OUT_APPEND)
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		mode = 0644;
		fd = open(redir->filename, flags, mode);
	}
	return (fd);
}

void	handle_redir_error(t_redir *redir)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->filename, STDERR_FILENO);
	if (redir->type == REDIR_IN)
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	else
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	exit(EXIT_GENERAL_ERROR);
}

void	update_redir_fd(t_redir *redir, int new_fd,
	int *input_fd, int *output_fd)
{
	if (redir->type == REDIR_IN)
	{
		if (*input_fd != -1)
			close(*input_fd);
		*input_fd = new_fd;
	}
	else
	{
		if (*output_fd != -1)
			close(*output_fd);
		*output_fd = new_fd;
	}
}

void	apply_final_redir(int input_fd, int output_fd)
{
	if (input_fd != -1)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != -1)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

void	setup_redir(t_cmd *cmd)
{
	t_redir	*redir;
	int		fd;
	int		input_fd;
	int		output_fd;

	if (!cmd || !cmd->redirs)
		return ;
	input_fd = -1;
	output_fd = -1;
	redir = cmd->redirs;
	while (redir)
	{
		fd = open_redir_file(redir);
		if (fd == -1)
		{
			if (input_fd != -1)
				close(input_fd);
			if (output_fd != -1)
				close(output_fd);
			handle_redir_error(redir);
		}
		update_redir_fd(redir, fd, &input_fd, &output_fd);
		redir = redir->next;
	}
	apply_final_redir(input_fd, output_fd);
}
