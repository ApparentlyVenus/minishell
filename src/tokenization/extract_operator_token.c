/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_operator_token.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 23:51:47 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 00:06:13 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token	*token_parenthesis(char *input, int *pos, t_token *token)
{
	if (input[*pos] == '(')
	{
		token->value = ft_substr(input, *pos, 1);
		token->type = TOKEN_LPAREN;
		(*pos)++;
		return (token);
	}
	else if (input[*pos] == ')')
	{
		token->value = ft_substr(input, *pos, 1);
		token->type = TOKEN_RPAREN;
		(*pos)++;
		return (token);
	}
	free(token);
	return (NULL);
}

static t_token	*token_left_redir(char *input, int *pos, t_token *token)
{
	if (input[*pos] == '<' && input[*pos + 1] == '<')
	{
		token->value = ft_substr(input, *pos, 2);
		token->type = TOKEN_HEREDOC;
		(*pos) += 2;
		return (token);
	}
	else if (input[*pos] == '<')
	{
		token->value = ft_substr(input, *pos, 1);
		token->type = TOKEN_REDIR_IN;
		(*pos)++;
		return (token);
	}
	free(token);
	return (NULL);
}

static t_token	*token_right_redir(char *input, int *pos, t_token *token)
{
	if (input[*pos] == '>' && input[*pos + 1] == '>')
	{
		token->value = ft_substr(input, *pos, 2);
		token->type = TOKEN_REDIR_OUT_APPEND;
		(*pos) += 2;
		return (token);
	}
	else if (input[*pos] == '>')
	{
		token->value = ft_substr(input, *pos, 1);
		token->type = TOKEN_REDIR_OUT;
		(*pos)++;
		return (token);
	}
	free(token);
	return (NULL);
}

static t_token	*token_pipe(char *input, int *pos, t_token *token)
{
	if (input[*pos] == '|' && input[*pos + 1] == '|')
	{
		token->value = ft_substr(input, *pos, 2);
		token->type = TOKEN_OR;
		token->priority = 1;
		(*pos) += 2;
		return (token);
	}
	else if (input[*pos] == '|')
	{
		token->value = ft_substr(input, *pos, 1);
		token->type = TOKEN_PIPE;
		token->priority = 2;
		(*pos)++;
		return (token);
	}
	free(token);
	return (NULL);
}

t_token	*extract_operator_token(char *input, int *pos)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->next = NULL;
	token->priority = 0;
	token->has_wildcard = 0;
	token->single_quotes = 0;
	token->double_quotes = 0;
	if (input[*pos] == '>')
		return (token_right_redir(input, pos, token));
	else if (input[*pos] == '|')
		return (token_pipe(input, pos, token));
	else if (input[*pos] == '<')
		return (token_left_redir(input, pos, token));
	else if (input[*pos] == '&' && input[*pos + 1] == '&')
		return (token->value = ft_substr(input, *pos, 2),
			token->type = TOKEN_AND, token->priority = 1,
			(*pos) += 2, token);
	else if (input[*pos] == '(' || input[*pos] == ')')
		return (token_parenthesis(input, pos, token));
	free(token);
	return (NULL);
}
