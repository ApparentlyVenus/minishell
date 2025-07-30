/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:38:58 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 00:06:47 by yitani           ###   ########.fr       */
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
	if (!token->single_quotes && !token->double_quotes)
		token->has_wildcard = has_wildcard(token->value);
	else
		token->has_wildcard = 0;
	return (token);
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
