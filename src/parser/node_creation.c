/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:33:54 by odana             #+#    #+#             */
/*   Updated: 2025/08/01 02:33:09 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*create_pipe_node(t_node *left, t_node *right)
{
	t_node	*pipe_node;

	pipe_node = (t_node *)malloc(sizeof(t_node));
	if (!pipe_node)
		return (free_node(left), free_node(right), NULL);
	pipe_node->type = NODE_PIPE;
	pipe_node->left = left;
	pipe_node->right = right;
	pipe_node->cmd = NULL;
	return (pipe_node);
}

t_node	*create_and_node(t_node *left, t_node *right)
{
	t_node	*and_node;

	and_node = (t_node *)malloc(sizeof(t_node));
	if (!and_node)
		return (free_node(left), free_node(right), NULL);
	and_node->type = NODE_AND;
	and_node->left = left;
	and_node->right = right;
	and_node->cmd = NULL;
	return (and_node);
}

t_node	*create_or_node(t_node *left, t_node *right)
{
	t_node	*or_node;

	or_node = (t_node *)malloc(sizeof(t_node));
	if (!or_node)
		return (free_node(left), free_node(right), NULL);
	or_node->type = NODE_OR;
	or_node->left = left;
	or_node->right = right;
	or_node->cmd = NULL;
	return (or_node);
}

t_node	*create_cmd_node(t_arg **args, t_redir *redir_list)
{
	t_cmd	*command;
	t_node	*node;

	command = (t_cmd *)malloc(sizeof(t_cmd));
	if (!command)
		return (free_split((char **)args), free_redir(redir_list), NULL);
	command->args = args;
	command->redirs = redir_list;
	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (free_cmd(command), NULL);
	node->type = NODE_CMD;
	node->cmd = command;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_redir	*create_redir_node(int type, char *filename)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (free(filename), NULL);
	redir->type = type;
	redir->filename = filename;
	redir->next = NULL;
	redir->expand_heredoc = 0;
	redir->single_quotes = 0;
	redir->double_quotes = 0; 
	return (redir);
}
