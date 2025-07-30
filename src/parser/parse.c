/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:54:08 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 01:14:59 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*parse_input(t_token **tokens, t_env *env)
{
	t_node	*ast;

	if (!tokens || !*tokens)
		return (NULL);
	ast = parse_pipeline(tokens, env, 1);
	if (!ast)
		return (NULL);
	if (*tokens)
	{
		free_node(ast);
		return (NULL);
	}
	return (ast);
}

t_node	*parse_pipeline(t_token **tokens, t_env *env, int min_priority)
{
	t_node			*left;
	t_op_info		op;

	left = parse_command_or_group(tokens, env);
	if (!left)
		return (NULL);
	while (*tokens)
	{
		op.priority = get_token_priority(*tokens);
		if (op.priority < min_priority || op.priority == 0)
			break ;
		op.type = (*tokens)->type;
		left = handle_operator(left, tokens, env, op);
		if (!left)
			return (NULL);
	}
	return (left);
}

t_node	*parse_group(t_token **tokens, t_env *env)
{
	t_node	*node;

	node = NULL;
	if (*tokens && (*tokens)->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next;
		node = parse_pipeline(tokens, env, 0);
		if (!node || !*tokens || (*tokens)->type != TOKEN_RPAREN)
		{
			if (node)
				free_node(node);
			return (NULL);
		}
		*tokens = (*tokens)->next;
		return (node);
	}
	return (NULL);
}
