/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:41:37 by odana             #+#    #+#             */
/*   Updated: 2025/07/27 12:54:10 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*collect_heredoc_content(char *delimiter, int expand, t_env *env)
{
	char	*line;
	char	*content;
	char	*expanded_line;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			expanded_line = expand_variables(line, env);
			if (!expanded_line)
				expanded_line = ft_strdup("");
		}
		else
			expanded_line = ft_strdup(line);
		if (expanded_line)
		{
			content = append_heredoc_line(content, expanded_line);
			free(expanded_line);
		}
		free(line);
		if (!content)
			return (NULL);
	}
	return (content);
}

char	*create_temp_file(char *content)
{
	char	*temp_filename;
	char	*suffix;
	char	*temp_base;
	int		fd;

	suffix = get_suffix();
	if (!suffix)
		return (NULL);
	temp_base = ft_strjoin("/tmp/heredoc_", suffix);
	free(suffix);
	if (!temp_base)
		return (NULL);
	temp_filename = ft_strdup(temp_base);
	free(temp_base);
	if (!temp_filename)
		return (NULL);
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (free(temp_filename), NULL);
	if (content && ft_strlen(content) > 0)
		write(fd, content, ft_strlen(content));
	close(fd);
	return (temp_filename);
}

t_redir	*process_heredoc(char *delimiter, t_env *env,
		int s_quotes, int d_quotes)
{
	char	*content;
	char	*temp_delimiter;
	char	*temp_filename;
	t_redir	*redir;

	if (!delimiter)
		return (NULL);
	temp_delimiter = ft_strdup(delimiter);
	if (!temp_delimiter)
		return (NULL);
	signal(SIGINT, SIG_DFL);
	content = collect_heredoc_content(temp_delimiter,
			!(s_quotes || d_quotes), env);
	free(temp_delimiter);
	signals_prompt();
	temp_filename = create_temp_file(content);
	free(content);
	if (!temp_filename)
		return (NULL);
	redir = create_redir_node(REDIR_IN, temp_filename);
	if (!redir)
		return (unlink(temp_filename), free(temp_filename), NULL);
	return (redir);
}
