/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:38:58 by yitani            #+#    #+#             */
/*   Updated: 2025/06/28 20:39:54 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// im displaying "minishell$" at the start of every prompt;
// eza the user enters CTRL + D this will put buffer to NULL
// thats why we should clean up and exit;
// clean up and exit doest have anything specified yet,
// it should contain the struct and all memory allocations;

#include "minishell.h"

int	get_input(char *stash)
{
	char	*buffer;

	buffer = NULL;
	buffer = readline("minishell$ ");
	if (!buffer)
	{
		return (clean_up_and_exit());
	}
	if (ft_strlen(buffer) > 0)
	{
		add_history(buffer);
		ft_strlcpy(stash, buffer, ft_strlen(buffer));
		free(buffer);
		return (1);
	}
	else
		return (0);
}

// in a shell a word can contain anything gher hol el seperators / tokens;
// even if mafi space bayneton its still a token, example :
//<infile cmd|cmd <outfile , should work perfectly fine;

// im extracting the words to tokenize them later in the next function;
// pos is the index [i] of the next function;

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

// Now we got the extracted word from quotes;
// 3anna meshkle wehde mabda2iyan;
// quotes preserved bel beginning and ending -> trim and clean;

t_token	*clean_word_token(char *word)
{
	t_token	*token;

	if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
		word = ft_strtrim(word, '\'');
	else if (word[0] == '\"' && word[ft_strlen(word) - 1] == '\"')
		word = ft_strtrim(word, '\"');
	token->value = word;
	token->type = TOKEN_WORD;
	return (token);
}

t_token	*extract_operator_token(char *input, int pos)
{
	t_token	*token;

	while (input[pos])
	{
		if (input[pos] == '>' && input[pos + 1] == '>')
			return (token->value = ft_substr(input, pos, 2),\
			token->type = TOKEN_REDIR_OUT_APPEND, token);
		else if (input[pos] == '>')
			return (token->value = ft_substr(input, pos, 1),\
			token->type = TOKEN_REDIR_OUT, token);
		else if (input[pos] == '|')
			return (token->value = ft_substr(input, pos, 1),\
			token->type = TOKEN_PIPE, token);
		else if (input[pos] == '<' && input[pos + 1] == '<')
			return (token->value = ft_substr(input, pos, 2),\
			token->type = TOKEN_HERDOC, token);
		else if (input[pos] == '<')
			return (token->value = ft_substr(input, pos, 1),\
			token->type = TOKEN_REDIR_IN, token);
	}
}

t_token	*tokenize_input(char *input)
{
	int		i;
	t_token	*token;
	char	*word;

	i = 0;
	while (input[i])
	{
		while (input[i] == 32 || (input[i] >= 9 && input[i] <= 13))
			i++;
		if (is_operator(input[i]))
			token = extract_operator_token(input, i);
		else if (is_quotes(input[i]) && is_closed(input))
		{
			word = extract_word(input, i);
			token = clean_word_token(word);
		}
		else if (is_quotes(input[i]) && !(is_closed(input)))
			ft_handle_later();
		else
		{
			word = extract_word(input, i);
			token = clean_word_token(word);
		}
	}
}
