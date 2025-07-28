/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:54:08 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 20:00:25 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
**  call pipeline, if there are trailing tokens free and return NULL
*/
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

/*
**  we need to build both a list of arguments and a list of redirections,
**  eventually we are going to convert the list of arguments to a double array
**  using the process_args function
**
** 2 Possible Patterns: 1. WORD+ redir*(at least one word, optional redirection)
**                          2. redir+ (at least one redirection)
*/
t_node	*parse_command(t_token **tokens, t_env *env)
{
	int		count;
	t_arg	*arg_list;
	t_redir	*redir_list;
	t_redir	*redir;

	count = 0;
	arg_list = NULL;
	redir_list = NULL;
	while (*tokens && ((*tokens)->type == TOKEN_WORD || is_redir(*tokens)))
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (!add_arg_list(&arg_list, *tokens))
				return (free_arg(arg_list), NULL);
			count++;
			*tokens = (*tokens)->next;
		}
		else if (is_redir(*tokens))
		{
			redir = parse_redir(tokens, env);
			if (!redir)
				return (free_arg(arg_list), free_redir(redir_list), (NULL));
			append_redir(&redir_list, redir);
		}
	}
	if (count == 0 && !redir_list)
		return (NULL);
	return (create_cmd_node(process_args(arg_list, count), redir_list));
}

t_redir	*parse_redir(t_token **tokens, t_env *env)
{
	int		type;
	char	*filename;
	t_token	*file_token;
	t_redir	*redir;

	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_REDIR_IN)
		type = REDIR_IN;
	else if ((*tokens)->type == TOKEN_REDIR_OUT)
		type = REDIR_OUT;
	else if ((*tokens)->type == TOKEN_REDIR_OUT_APPEND)
		type = REDIR_OUT_APPEND;
	else if ((*tokens)->type == TOKEN_HEREDOC)
		type = HERE_DOC;
	else
		return (NULL);
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	file_token = *tokens;
	filename = (*tokens)->value;
	*tokens = (*tokens)->next;
	if (type == HERE_DOC)
	{
		redir = process_heredoc(filename, env,
				file_token->single_quotes, file_token->double_quotes);
		if (!redir)
			return (NULL);
		return (redir);
	}
	return (create_redir_node(type, ft_strdup(filename)));
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
