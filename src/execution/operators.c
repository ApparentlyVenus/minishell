/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 07:36:25 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 22:30:35 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_and(t_node *node, t_shell *shell)
{
	int	left_exit;
	int	right_exit;

	if (run_in_parent(node->left))
		left_exit = execute_parent_node(node->left, shell);
	else
		left_exit = execute_children_node(node->left, shell);
	if (left_exit == EXIT_SUCCESS)
	{
		if (run_in_parent(node->right))
			right_exit = execute_parent_node(node->right, shell);
		else
			right_exit = execute_children_node(node->right, shell);
	}
	else
		right_exit = left_exit;
	return (right_exit);
}

int	execute_or(t_node *node, t_shell *shell)
{
	int	left_exit;
	int	right_exit;

	if (run_in_parent(node->left))
		left_exit = execute_parent_node(node->left, shell);
	else
		left_exit = execute_children_node(node->left, shell);
	if (left_exit == EXIT_CTRL_C || g_signal_received == SIGINT)
		return (left_exit);
	if (left_exit != EXIT_SUCCESS)
	{
		if (run_in_parent(node->right))
			right_exit = execute_parent_node(node->right, shell);
		else
			right_exit = execute_children_node(node->right, shell);
	}
	else
		right_exit = left_exit;
	return (right_exit);
}
