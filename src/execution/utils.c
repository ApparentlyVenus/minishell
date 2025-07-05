/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:52:31 by odana             #+#    #+#             */
/*   Updated: 2025/07/05 12:37:42 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*find_path(char *cmd, t_env *env_list)
{
	char	*env_path;
	char	**paths;
	char	*full_path;
	char	*part_path;
	int		i;

	env_path = get_env_value(env_list, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (full_path && access(full_path, X_OK) == 0)
			return (free_split(paths), full_path);
		free(full_path);
	}
	return (free_split(paths), NULL);
}

int	count_commands(t_node *node)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (count_commands(node->left) + count_commands(node->right));
	if (node->type == NODE_CMD)
		return (1);
	return (0);
}

int	wait_for_children(t_exec *ctx)
{
	int	status;
	int	exit_code;
	int	i;

	status = 0;
	exit_code = 0;
	i = 0;
	while (i < ctx->cmd_count)
	{
		waitpid(ctx->pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		i++;
	}
	return (exit_code);
}

/*
** get_nth_command - Retrieves the nth command node from pipeline
**
** Navigates the left-associative AST to find a specific command
** Used to iterate through commands in execution order
*/
t_node	*get_nth_command(t_node *node, int n)
{
	int	left_count;

	if (!node)
		return (NULL);
	if (node->type == NODE_PIPE)
	{
		left_count = count_commands(node->left);
		if (n < left_count)
			return (get_nth_command(node->left, n));
		else
			return (get_nth_command(node->right, n - left_count));
	}
	if (node->type == NODE_CMD && n == 0)
		return (node);
	return (NULL);
}

t_builtin	get_builtin_type(const char *cmd_name)
{
	if (!cmd_name)
		return (BUILTIN_NONE);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (BUILTIN_EXIT);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (BUILTIN_UNSET);
	return (BUILTIN_NONE);
}
