/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:38:58 by yitani            #+#    #+#             */
/*   Updated: 2025/07/07 07:13:01 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*extract_word(char *input, int *pos)
{
	int		i;
	int		len;
	int		in_single_quotes;
	int		in_double_quotes;
	char	*word;

	i = *pos;
	len = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[*pos])
	{
		if (input[*pos] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[*pos] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (!in_single_quotes && !in_double_quotes
			&& !is_word_char(input[*pos]))
			break ;
		(*pos)++;
		len++;
	}
	word = ft_substr(input, i, len);
	return (word);
}

t_token	*clean_word_token(char *word)
{
	t_token	*token;
	char	*trimmed;

	token = malloc(sizeof(t_token));
	token->single_quotes = 0;
	token->double_quotes = 0;
	if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
	{
		token->single_quotes = 1;
		trimmed = ft_strtrim(word, '\'');
		free(word);
		word = trimmed;
	}
	else if (word[0] == '"' && word[ft_strlen(word) - 1] == '"')
	{
		token->double_quotes = 1;
		trimmed = ft_strtrim(word, '"');
		free(word);
		word = trimmed;
	}
	token->value = word;
	token->type = TOKEN_WORD;
	token->priority = 0;
	token->has_wildcard = has_wildcard(token->value);
	return (token->next = NULL, token);
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
	while (input[*pos])
	{
		if (input[*pos] == '>' && input[*pos + 1] == '>')
			return (token->value = ft_substr(input, *pos, 2),
				token->type = TOKEN_REDIR_OUT_APPEND, (*pos) += 2, token);
		else if (input[*pos] == '>')
			return (token->value = ft_substr(input, *pos, 1),
				token->type = TOKEN_REDIR_OUT, (*pos)++, token);
		else if (input[*pos] == '|' && input[*pos + 1] == '|')
			return (token->value = ft_substr(input, *pos, 2),
				token->type = TOKEN_OR, token->priority = 1, (*pos) += 2, token);
		else if (input[*pos] == '<' && input[*pos + 1] == '<')
			return (token->value = ft_substr(input, *pos, 2),
				token->type = TOKEN_HERDOC, (*pos) += 2, token);
		else
			token = extract_bonus_token(input, pos, token);
		if (!token)
			return (NULL);
		else
			return (token);
	}
}

t_token	**tokenize_input(char *input, t_token **token)
{
	int		i;
	t_token	*new_token;
	char	*word;

	i = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (is_operator(input[i]))
			new_token = extract_operator_token(input, &i);
		if (is_quotes(input[i]) || is_word_char(input[i]))
			new_token = handle_any_word(input, word, &i, new_token);
		if (!new_token)
		{
			free_tokens(token);
			return (NULL);
		}
		ft_lstadd_back(token, new_token);
		new_token = NULL;
	}
	return (token);
}
