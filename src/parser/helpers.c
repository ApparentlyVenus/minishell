/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 08:18:30 by odana             #+#    #+#             */
/*   Updated: 2025/07/07 08:21:39 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*parse_command_or_group(t_token **tokens)
{
	t_node	*node;

	node = parse_group(tokens);
	if (node)
		return (node);
	return (parse_command(tokens));
}

t_node	*handle_operator(t_node *left, t_token **tokens,
					t_token_type op_type, int op_priority)
{
	t_node	*right;

	*tokens = (*tokens)->next;
	right = parse_pipeline(tokens, op_priority + 1);
	if (!right)
		return (free_node(left), NULL);
	if (op_type == TOKEN_PIPE)
		return (create_pipe_node(left, right));
	else if (op_type == TOKEN_AND)
		return (create_and_node(left, right));
	else if (op_type == TOKEN_OR)
		return (create_or_node(left, right));
	return (free_node(left), free_node(right), NULL);
}
