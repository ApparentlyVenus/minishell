/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:41:37 by odana             #+#    #+#             */
/*   Updated: 2025/07/18 12:54:35 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** collect_heredoc_content
** Purpose: Collects heredoc input from user until delimiter is found
** Used variables: delimiter (stop word)
** Return: Newly allocated string with all heredoc content
*/
char	*collect_heredoc_content(char *delimiter)
{
	char	*line;
	char	*content;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
            write(STDOUT_FILENO, "\n", 1);
			break;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		content = append_heredoc_line(content, line);
		free(line);
		if (!content)
			return (NULL);
	}
	
	return (content);
}

/*
** create_temp_file
** Purpose: Creates a temporary file and writes heredoc content to it
** Used variables: content (heredoc text)
** Return: Newly allocated filename string
*/
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

/*
** process_heredoc
** Purpose: Main function to process heredoc during parsing
** Used variables: delimiter (heredoc delimiter)
** Return: Newly allocated t_redir with temp file as filename
*/
t_redir	*process_heredoc(char *delimiter)
{
	char	*unquoted_delimiter;
	char	*content;
	char	*temp_filename;
	t_redir	*redir;

	if (!delimiter)
		return (NULL);
	unquoted_delimiter = unquote_delimiter(delimiter);
	if (!unquoted_delimiter)
		return (NULL);
    signal(SIGINT, SIG_DFL);
	content = collect_heredoc_content(unquoted_delimiter);
	free(unquoted_delimiter);
	signals_prompt();
	if (!content)
		return (NULL);
	temp_filename = create_temp_file(content);
	free(content);
	if (!temp_filename)
		return (NULL);
	redir = create_redir_node(REDIR_IN, temp_filename);
	if (!redir)
		return (unlink(temp_filename), free(temp_filename), NULL);
	return (redir);
}

/*
** cleanup_heredoc_files
** Purpose: Removes temporary heredoc files (call during cleanup)
** Used variables: cmd (command with redirections)
** Return: None
*/
void	cleanup_heredoc_files(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return;
	
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN && redir->filename &&
                ft_strncmp(redir->filename, "/tmp/heredoc_", 13) == 0)
			unlink(redir->filename); 
		redir = redir->next;
	}
}
