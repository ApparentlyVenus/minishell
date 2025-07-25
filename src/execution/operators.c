/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 07:36:25 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 07:37:33 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_and(t_node *node, t_shell *shell,
		int (*executor)(t_node *, t_shell *))
{
	int	left_exit;
	int	right_exit;

	left_exit = executor(node->left, shell);
	if (left_exit == EXIT_SUCCESS)
		right_exit = executor(node->right, shell);
	else
		right_exit = left_exit;
	return (right_exit);
}

int	execute_or(t_node *node, t_shell *shell,
		int (*executor)(t_node *, t_shell *))
{
	int	left_exit;
	int	right_exit;

	left_exit = executor(node->left, shell);
	if (left_exit != EXIT_SUCCESS)
		right_exit = executor(node->right, shell);
	else
		right_exit = left_exit;
	return (right_exit);
}
