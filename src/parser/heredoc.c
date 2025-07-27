/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:41:37 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 01:58:27 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int g_heredoc_interrupted = 0;

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	write(STDOUT_FILENO, "\n", 1);
	// Send SIGUSR1 to ourselves to interrupt readline
	kill(getpid(), SIGUSR1);
}

void	handle_sigusr1(int sig)
{
	(void)sig;
	// This handler does nothing, just interrupts readline
}

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
		if (g_heredoc_interrupted)
		{
			free(content);
			return (NULL);
		}
		line = readline("heredoc> ");
		if (g_heredoc_interrupted)
		{
			if (line)
				free(line);
			free(content);
			return (NULL);
		}
		if (!line)
		{
			if (g_heredoc_interrupted)
			{
				free(content);
				return (NULL);
			}
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

// t_redir	*process_heredoc(char *delimiter, t_env *env,
// 		int s_quotes, int d_quotes)
// {
// 	char	*content;
// 	char	*temp_delimiter;
// 	char	*temp_filename;
// 	t_redir	*redir;

// 	if (!delimiter)
// 		return (NULL);
// 	temp_delimiter = ft_strdup(delimiter);
// 	if (!temp_delimiter)
// 		return (NULL);
// 	content = collect_heredoc_content(temp_delimiter,
// 			!(s_quotes || d_quotes), env);
// 	free(temp_delimiter);
// 	signals_prompt();
// 	if (!content)
// 		return (NULL);
// 	temp_filename = create_temp_file(content);
// 	free(content);
// 	if (!temp_filename)
// 		return (NULL);
// 	redir = create_redir_node(REDIR_IN, temp_filename);
// 	if (!redir)
// 		return (unlink(temp_filename), free(temp_filename), NULL);
// 	return (redir);
// }

// t_redir	*process_heredoc(char *delimiter, t_env *env,
// 		int s_quotes, int d_quotes)
// {
// 	char	*content;
// 	char	*temp_delimiter;
// 	char	*temp_filename;
// 	t_redir	*redir;

// 	if (!delimiter)
// 		return (NULL);
// 	temp_delimiter = ft_strdup(delimiter);
// 	if (!temp_delimiter)
// 		return (NULL);
	
// 	g_heredoc_interrupted = 0;
// 	signal(SIGINT, handle_heredoc_sigint);
	
// 	content = collect_heredoc_content(temp_delimiter,
// 			!(s_quotes || d_quotes), env);
	
// 	signal(SIGINT, handle_sigint_prompt); // Restore normal handler
	
// 	free(temp_delimiter);
// 	if (!content) // Heredoc was interrupted or failed
// 		return (NULL);
// 	temp_filename = create_temp_file(content);
// 	free(content);
// 	if (!temp_filename)
// 		return (NULL);
// 	redir = create_redir_node(REDIR_IN, temp_filename);
// 	if (!redir)
// 		return (unlink(temp_filename), free(temp_filename), NULL);
// 	return (redir);
// }




// t_redir	*process_heredoc(char *delimiter, t_env *env,
// 		int s_quotes, int d_quotes)
// {
// 	t_heredoc_data	data;
// 	char			*temp_filename;
// 	t_redir			*redir;
// 	pid_t			pid;
// 	int				status;
// 	int				pipe_fd[2];

// 	if (!delimiter)
// 		return (NULL);
// 	data.delimiter = ft_strdup(delimiter);
// 	if (!data.delimiter)
// 		return (NULL);
// 	data.env = env;
// 	data.s_quotes = s_quotes;
// 	data.d_quotes = d_quotes;
// 	data.pipe_fd = pipe_fd;
// 	if (pipe(pipe_fd) == -1)
// 		return (free(data.delimiter), NULL);
	
// 	pid = fork();
// 	if (pid == 0)
// 		heredoc_child_process(&data);
// 	else if (pid > 0)
// 	{
// 		free(data.delimiter);
// 		waitpid(pid, &status, 0);
		
// 		// Reset readline's signal handling after fork
// 		rl_clear_history();
// 		rl_on_new_line();
		
// 		// Explicitly reinstall your signal handlers
// 		signal(SIGINT, handle_sigint_prompt);
// 		signal(SIGQUIT, SIG_IGN);
		
// 		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 		{
// 			close(pipe_fd[0]);
// 			close(pipe_fd[1]);
// 			return (NULL);
// 		}
// 		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
// 		{
// 			temp_filename = read_filename_from_child(pipe_fd);
// 			if (!temp_filename)
// 				return (NULL);
// 			redir = create_redir_node(REDIR_IN, temp_filename);
// 			if (!redir)
// 				return (unlink(temp_filename), free(temp_filename), NULL);
// 			return (redir);
// 		}
// 		else
// 		{
// 			close(pipe_fd[0]);
// 			close(pipe_fd[1]);
// 			return (NULL);
// 		}
// 	}
// 	else
// 	{
// 		free(data.delimiter);
// 		close(pipe_fd[0]);
// 		close(pipe_fd[1]);
// 		return (NULL);
// 	}
// 	return (NULL);
// }

// t_redir	*process_heredoc(char *delimiter, t_env *env,
// 		int s_quotes, int d_quotes)
// {
// 	char	*content;
// 	char	*temp_delimiter;
// 	char	*temp_filename;
// 	t_redir	*redir;

// 	if (!delimiter)
// 		return (NULL);
// 	temp_delimiter = ft_strdup(delimiter);
// 	if (!temp_delimiter)
// 		return (NULL);
	
// 	g_heredoc_interrupted = 0;
// 	signal(SIGINT, handle_heredoc_sigint);
	
// 	content = collect_heredoc_content(temp_delimiter,
// 			!(s_quotes || d_quotes), env);
	
// 	signal(SIGINT, handle_sigint_prompt);
	
// 	free(temp_delimiter);
// 	if (!content)
// 		return (NULL);
// 	temp_filename = create_temp_file(content);
// 	free(content);
// 	if (!temp_filename)
// 		return (NULL);
// 	redir = create_redir_node(REDIR_IN, temp_filename);
// 	if (!redir)
// 		return (unlink(temp_filename), free(temp_filename), NULL);
// 	return (redir);
// }

t_redir	*process_heredoc(char *delimiter, t_env *env,
		int s_quotes, int d_quotes)
{
	t_heredoc_data	data;
	char			*temp_filename;
	t_redir			*redir;
	pid_t			pid;
	int				status;
	int				pipe_fd[2];

	if (!delimiter)
		return (NULL);
	data.delimiter = ft_strdup(delimiter);
	if (!data.delimiter)
		return (NULL);
	data.env = env;
	data.s_quotes = s_quotes;
	data.d_quotes = d_quotes;
	data.pipe_fd = pipe_fd;
	if (pipe(pipe_fd) == -1)
		return (free(data.delimiter), NULL);
	
	pid = fork();
	if (pid == 0)
		heredoc_child_process(&data);
	else if (pid > 0)
	{
		free(data.delimiter);
		waitpid(pid, &status, 0);
		
		// CRITICAL: Reset signals completely then restore
		reset_signals();
		signals_prompt();
		
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (NULL);
		}
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			temp_filename = read_filename_from_child(pipe_fd);
			if (!temp_filename)
				return (NULL);
			redir = create_redir_node(REDIR_IN, temp_filename);
			if (!redir)
				return (unlink(temp_filename), free(temp_filename), NULL);
			return (redir);
		}
		else
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (NULL);
		}
	}
	else
	{
		reset_signals();
		signals_prompt();
		free(data.delimiter);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (NULL);
	}
	return (NULL);
}
