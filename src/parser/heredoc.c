/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:41:37 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 20:08:55 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	setup_heredoc_fork(int pipe_fd[2], char *delimiter, int expand, t_env *env)
{
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	signals_parent();
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		signals_prompt();
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		signals_child();
		heredoc_child_process(pipe_fd[1], delimiter, expand, env);
	}
	close(pipe_fd[1]);
	return (pid);
}

char	*collect_heredoc_content(char *delimiter, int expand, t_env *env)
{
	int		pipe_fd[2];
	char	*content;
	int		status;
	pid_t	pid;

	pid = setup_heredoc_fork(pipe_fd, delimiter, expand, env);
	if (pid == -1)
		return (NULL);
	waitpid(pid, &status, 0);
	signals_prompt();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		return ((char *)-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 42)
	{
		close(pipe_fd[0]);
		return (NULL);
	}
	content = read_from_pipe(pipe_fd[0]);
	close(pipe_fd[0]);
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

void	CTRL_D_ERR(char *del)
{
	printf("here-document delimited by end-of-file (wanted `%s')\n", del);
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
	content = collect_heredoc_content(temp_delimiter,
			!(s_quotes || d_quotes), env);
	free(temp_delimiter);
	if (content == (char *) - 1)
		return (printf("\n"), NULL);
	if (!content)
		return(CTRL_D_ERR(delimiter), NULL);
	temp_filename = create_temp_file(content);
	free(content);
	if (!temp_filename)
		return (NULL);
	redir = create_redir_node(REDIR_IN, temp_filename);
	if (!redir)
		return (unlink(temp_filename), free(temp_filename), NULL);
	return (redir);
}
