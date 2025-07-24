/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:43:57 by odana             #+#    #+#             */
/*   Updated: 2025/07/24 23:56:20 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** is_delimiter_quoted
** Purpose: Checks if heredoc delimiter was quoted (affects expansion)
** Used variables: delimiter (string to check)
** Return: 1 if quoted, 0 if not
*/
int	is_delimiter_quoted(char *delimiter)
{
	int	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2
		&& ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\'')))
		return (1);
	return (0);
}

/*
** unquote_delimiter
** Purpose: Removes quotes from delimiter if present
** Used variables: delimiter (quoted string)
** Return: Newly allocated unquoted string
*/
char	*unquote_delimiter(char *delimiter)
{
	int		len;
	char	*unquoted;

	if (!is_delimiter_quoted(delimiter))
		return (ft_strdup(delimiter));
	len = ft_strlen(delimiter);
	unquoted = ft_substr(delimiter, 1, len - 2);
	return (unquoted);
}

/*
** append_heredoc_line
** Purpose: Appends a line to heredoc content with newline
** Used variables: content (existing), line (new line)
** Return: Newly allocated combined string
*/
char	*append_heredoc_line(char *content, char *line)
{
	char	*line_with_newline;
	char	*result;

	if (!content)
		content = ft_strdup("");
	if (!content)
		return (NULL);
	line_with_newline = ft_strjoin(line, "\n");
	if (!line_with_newline)
		return (free(content), NULL);
	result = ft_strjoin(content, line_with_newline);
	free(content);
	free(line_with_newline);
	return (result);
}

char	*get_suffix(void)
{
	static int	counter = 0;

	counter++;
	return (ft_itoa(counter));
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
		return ;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN && redir->filename
			&& ft_strncmp(redir->filename, "/tmp/heredoc_", 13) == 0)
			unlink(redir->filename);
		redir = redir->next;
	}
}
