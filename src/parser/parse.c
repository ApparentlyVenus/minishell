/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:54:08 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 09:37:35 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
**  call pipeline, if there are trailing tokens free and return NULL
*/
t_node	*parse_input(t_token **tokens)
{
	t_node	*ast;

	if (!tokens || !*tokens)
		return (NULL);
	ast = parse_pipeline(tokens, 1);
	if (!ast)
		return (NULL);
	if (*tokens)
	{
		free_node(ast);
		return (NULL);
	}
	return (ast);
}

t_node	*parse_pipeline(t_token **tokens, int min_priority)
{
	t_node			*left;
	int				op_priority;
	t_token_type	op_type;

	left = parse_command_or_group(tokens);
	if (!left)
		return (NULL);
	while (*tokens)
	{
		op_priority = get_token_priority(*tokens);
		if (op_priority < min_priority || op_priority == 0)
			break ;
		op_type = (*tokens)->type;
		left = handle_operator(left, tokens, op_type, op_priority);
		if (!left)
			return (NULL);
	}
	return (left);
}

/*
**  we need to build both a list of arguments and a list of redirections,
**  eventually we are going to convert the list of arguments to a double array
**  using the process_args function
**
** 2 Possible Patterns: 1. WORD+ redir*(at least one word, optional redirection)
**                          2. redir+ (at least one redirection)
*/
t_node	*parse_command(t_token **tokens)
{
	int		count;
	t_arg	*arg_list;
	t_redir	*redir_list;
	t_redir	*redir;

	count = 0;
	arg_list = NULL;
	redir_list = NULL;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		if (!add_arg_list(&arg_list, *tokens))
			return (free_arg(arg_list), NULL);
		count++;
		*tokens = (*tokens)->next;
	}
	while (*tokens && is_redir(*tokens))
	{
		redir = parse_redir(tokens);
		if (!redir)
			return (free_arg(arg_list), free_redir(redir_list), (NULL));
		append_redir(&redir_list, redir);
	}
	if (count == 0 && !redir_list)
		return (NULL);
	return (create_cmd_node(process_args(arg_list, count), redir_list));
}

t_redir	*parse_redir(t_token **tokens)
{
	int		type;
	char	*filename;
	t_redir	*redir;

	if (!*tokens)
		return (NULL);
	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	filename = (*tokens)->value;
	*tokens = (*tokens)->next;
	if (type == TOKEN_HEREDOC)
	{
		redir = process_heredoc(filename);
		return (redir);
	}
	return (create_redir_node(type, ft_strdup(filename)));
}

t_node	*parse_group(t_token **tokens)
{
	t_node	*node;

	node = NULL;
	if (*tokens && (*tokens)->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next;
		node = parse_pipeline(tokens, 0);
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
