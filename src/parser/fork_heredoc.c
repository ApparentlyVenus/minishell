/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:25:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 00:07:15 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*read_heredoc_line(void)
{
	char	buffer[1024];
	char	*line;
	ssize_t	bytes_read;
	int		i;

	write(STDOUT_FILENO, "heredoc> ", 9);
	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (NULL);
	}
	buffer[bytes_read] = '\0';
	i = 0;
	while (i < bytes_read && buffer[i] != '\n')
		i++;
	if (i < bytes_read && buffer[i] == '\n')
		buffer[i] = '\0';
	line = ft_strdup(buffer);
	return (line);
}

static char	*process_heredoc_line(char *line, int expand, t_env *env)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_variables(line, env);
		if (!expanded_line)
			expanded_line = ft_strdup("");
	}
	else
		expanded_line = ft_strdup(line);
	return (expanded_line);
}

void	heredoc_child_process(int pipe_fd, char *delimiter, int expand,
	t_env *env)
{
	char	*line;
	char	*expanded_line;

	signals_child();
	while (1)
	{
		line = read_heredoc_line();
		if (!line)
			return (close(pipe_fd), exit(42));
		if (ft_strcmp(line, delimiter) == 0)
			return (free(line), close(pipe_fd), exit(0));
		expanded_line = process_heredoc_line(line, expand, env);
		if (expanded_line)
		{
			write(pipe_fd, expanded_line, ft_strlen(expanded_line));
			write(pipe_fd, "\n", 1);
			free(expanded_line);
		}
		free(line);
	}
}

char	*read_from_pipe(int pipe_fd)
{
	char	*content;
	char	*temp;
	char	buffer[1024];
	ssize_t	bytes_read;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	bytes_read = read(pipe_fd, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		free(content);
		if (!temp)
			return (NULL);
		content = temp;
		bytes_read = read(pipe_fd, buffer, sizeof(buffer) - 1);
	}
	return (content);
}
