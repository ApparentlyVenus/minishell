/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:38:58 by yitani            #+#    #+#             */
/*   Updated: 2025/07/25 09:14:28 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*extract_word(char *input, int *pos)
{
	int		i;
	int		len;
	int		in_single_quotes;
	int		in_double_quotes;

	i = *pos;
	len = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[*pos])
	{
		toggle_quotes(input[*pos], &in_single_quotes, &in_double_quotes);
		if (!continue_word(input[*pos], in_single_quotes, in_double_quotes))
			break ;
		(*pos)++;
		len++;
	}
	return (ft_substr(input, i, len));
}

t_token	*clean_word_token(char *word)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->single_quotes = 0;
	token->double_quotes = 0;
	token->next = NULL;
	word = trim_quotes(word, token);
	if (!word)
		return (free(token), NULL);
	token->value = word;
	token->type = TOKEN_WORD;
	token->priority = 0;
	token->has_wildcard = has_wildcard(token->value);
	return (token);
}

t_token	*extract_bonus_token(char *input, int *pos, t_token *token)
{
	if (input[*pos] == '|')
		return (token->value = ft_substr(input, *pos, 1),
			token->type = TOKEN_PIPE, token->priority = 2, (*pos)++, token);
	else if (input[*pos] == '*')
		return (token->value = ft_substr(input, *pos, 1),
			token->type = TOKEN_WILDCARD, (*pos)++, token);
	else if (input[*pos] == '&' && input[*pos + 1] == '&')
		return (token->value = ft_substr(input, *pos, 2),
			token->type = TOKEN_AND, token->priority = 1, (*pos) += 2, token);
	else if (input[*pos] == '<')
		return (token->value = ft_substr(input, *pos, 1),
			token->type = TOKEN_REDIR_IN, (*pos)++, token);
	else if (input[*pos] == '(')
		return (token->value = ft_substr(input, *pos, 1),
			token->type = TOKEN_LPAREN, (*pos)++, token);
	else if (input[*pos] == ')')
		return (token->value = ft_substr(input, *pos, 1),
			token->type = TOKEN_RPAREN, (*pos)++, token);
	else
		return (free(token), NULL);
}

t_token	*extract_operator_token(char *input, int *pos)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->next = NULL;
	if (input[*pos] == '>' && input[*pos + 1] == '>')
		return (token->value = ft_substr(input, *pos, 2),
			token->type = TOKEN_REDIR_OUT_APPEND, (*pos) += 2, token);
	else if (input[*pos] == '>')
		return (token->value = ft_substr(input, *pos, 1),
			token->type = TOKEN_REDIR_OUT, (*pos)++, token);
	else if (input[*pos] == '|' && input[*pos + 1] == '|')
		return (token->value = ft_substr(input, *pos, 2),
			token->type = TOKEN_OR, token->priority = 1, *pos += 2, token);
	else if (input[*pos] == '<' && input[*pos + 1] == '<')
		return (token->value = ft_substr(input, *pos, 2),
			token->type = TOKEN_HEREDOC, (*pos) += 2, token);
	else
	{
		token = extract_bonus_token(input, pos, token);
		if (!token)
			return (NULL);
		return (token);
	}
	return (free(token), NULL);
}

t_token	**tokenize_input(char *input, t_shell *shell)
{
	int		i;
	t_token	*new_token;

	i = 0;
	shell->tokens = NULL;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		new_token = create_next_token(input, &i, shell);
		if (!new_token)
			return (free_tokens(&shell->tokens), NULL);
		token_add_back(&shell->tokens, new_token);
	}
	return (&shell->tokens);
}
