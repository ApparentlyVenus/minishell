/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 00:16:09 by yitani            #+#    #+#             */
/*   Updated: 2025/07/28 01:55:10 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	heredoc_child_process(t_heredoc_data *data)
{
	char	*content;
	char	*temp_filename;

	close(data->pipe_fd[0]);
	reset_signals(); // Reset to default signals in child
	
	content = collect_heredoc_content(data->delimiter, 
			!(data->s_quotes || data->d_quotes), data->env);
	if (!content)
		exit(1);
	temp_filename = create_temp_file(content);
	free(content);
	if (!temp_filename)
		exit(1);
	write(data->pipe_fd[1], temp_filename, ft_strlen(temp_filename));
	close(data->pipe_fd[1]);
	free(temp_filename);
	exit(0);
}

char	*read_filename_from_child(int pipe_fd[2])
{
	char	buffer[1024];
	int		bytes_read;
	char	*filename;

	close(pipe_fd[1]);
	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	close(pipe_fd[0]);
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	filename = ft_strdup(buffer);
	return (filename);
}
