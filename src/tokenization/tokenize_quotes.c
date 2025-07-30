/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 23:49:12 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 02:43:00 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	has_adjacent_quotes(char *input, int pos)
{
	int	i;
	int	quote_count;
	int	in_single;
	int	in_double;

	i = pos;
	quote_count = 0;
	in_single = 0;
	in_double = 0;
	while (input[i] && (is_word_char(input[i]) || is_quotes(input[i])
			|| input[i] == '*'))
	{
		if (input[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			quote_count++;
		}
		else if (input[i] == '"' && !in_single)
		{
			in_double = !in_double;
			quote_count++;
		}
		i++;
	}
	return (quote_count > 2);
}

char	*extract_quote_segment(char *input, int *pos, int *s_quote, int *d_quote)
{
	int		start;
	char	quote_char;
	char	*result;

	*s_quote = 0;
	*d_quote = 0;
	if (!is_quotes(input[*pos]))
	{
		start = *pos;
		while (input[*pos] && !is_quotes(input[*pos]) && is_word_char(input[*pos]))
			(*pos)++;
		return (ft_substr(input, start, *pos - start));
	}
	quote_char = input[*pos];
	(*pos)++;
	start = *pos;
	while (input[*pos] && input[*pos] != quote_char)
		(*pos)++;
	if (input[*pos] != quote_char)
		return (NULL);
	result = ft_substr(input, start, *pos - start);
	(*pos)++;
	if (quote_char == '\'')
		*s_quote = 1;
	else
		*d_quote = 1;
	return (result);
}

t_token	*create_segment_token(char *segment, int s_quote, int d_quote)
{
	t_token	*new_token;
	int		has_wild;

	if (!segment || ft_strlen(segment) == 0)
		return (free(segment), NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (free(segment), NULL);
	new_token->type = TOKEN_WORD;
	new_token->value = segment;
	new_token->single_quotes = s_quote;
	new_token->double_quotes = d_quote;
	new_token->priority = 0;
	has_wild = 0;
	if (!s_quote && !d_quote)
		has_wild = has_wildcard(segment);
	new_token->has_wildcard = has_wild;
	new_token->next = NULL;
	return (new_token);
}

t_token	*tokenize_complex_word(char *input, int *i, t_shell *shell)
{
	t_token	*first_token;
	t_token	*current_token;
	t_token	*new_token;
	char	*segment;
	int		quotes[2];

	first_token = NULL;
	current_token = NULL;
	while (input[*i] && (is_quotes(input[*i]) || is_word_char(input[*i])))
	{
		segment = extract_quote_segment(input, i, &quotes[0], &quotes[1]);
		if (!segment)
			return (free_tokens(&first_token), 
				handle_error(shell, "unclosed quotes", EXIT_MISUSE), NULL);
		new_token = create_segment_token(segment, quotes[0], quotes[1]);
		if (!new_token)
			continue ;
		if (!first_token)
			first_token = new_token;
		else
			current_token->next = new_token;
		current_token = new_token;
	}
	return (first_token);
}

void	add_tokens(t_token *new_token, t_shell *shell, int had_space)
{
	t_token	*token_list;
	t_token	*current;
	int		is_first_in_group;

	if (new_token->next)
	{
		token_list = new_token;
		is_first_in_group = 1;
		while (token_list)
		{
			current = token_list;
			token_list = token_list->next;
			current->next = NULL;
			if (is_first_in_group)
			{
				current->concat = !had_space && shell->tokens;
				is_first_in_group = 0;
			}
			else
				current->concat = 1;
			token_add_back(&shell->tokens, current);
		}
	}
	else
	{
		new_token->concat = !had_space && shell->tokens;
		token_add_back(&shell->tokens, new_token);
	}
}
