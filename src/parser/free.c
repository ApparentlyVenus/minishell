/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 08:27:03 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 21:35:01 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_split(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			if (cmd->args[i]->value)
				free(cmd->args[i]->value);
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free_redir(cmd->redirs);
	free(cmd);
}

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void	free_arg(t_arg *arg_list)
{
	t_arg	*temp;

	while (arg_list)
	{
		temp = arg_list->next;
		free(arg_list->value);
		free(arg_list);
		arg_list = temp;
	}
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	free_node(node->left);
	free_node(node->right);
	free_cmd(node->cmd);
	free(node);
}
