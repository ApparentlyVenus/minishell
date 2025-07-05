/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 08:27:03 by odana             #+#    #+#             */
/*   Updated: 2025/07/05 12:52:51 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** the thing here with freeing is that to free a tree you need to call
** the left and right branches recursively
** and because we have multiple we need a function for each struct
!! each struct's free function will call the free 
	function of the structs inside it
*/

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
	if (!cmd)
		return ;
	free_split(cmd->args);
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
