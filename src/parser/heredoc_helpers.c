/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:43:57 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 21:35:20 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

char	*get_suffix(void)
{
	static int	counter = 0;

	counter++;
	return (ft_itoa(counter));
}

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
