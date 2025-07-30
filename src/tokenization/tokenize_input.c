/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:38:58 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 02:47:06 by odana            ###   ########.fr       */
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

void	add_token_list(t_token *token_list, t_shell *shell)
{
	t_token	*current;
	t_token	*next;

	current = token_list;
	while (current)
	{
		next = current->next;
		current->next = NULL;
		token_add_back(&shell->tokens, current);
		current = next;
	}
}

t_token	**tokenize_input(char *input, t_shell *shell)
{
	int		i;
	int		had_space;
	t_token	*new_token;

	i = 0;
	shell->tokens = NULL;
	while (input[i])
	{
		had_space = 0;
		if (input[i] == ' ' || input[i] == '\t'
			|| input[i] == '\n' || input[i] == '\r')
			had_space = 1;
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		new_token = create_next_token(input, &i, shell);
		if (!new_token)
			return (free_tokens(&shell->tokens), NULL);
		add_tokens(new_token, shell, had_space);
	}
	return (&shell->tokens);
}

int	valid_quotes(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			if (!current->value)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
