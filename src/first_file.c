/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:38:58 by yitani            #+#    #+#             */
/*   Updated: 2025/06/28 06:35:54 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// im displaying "minishell$" at the start of every prompt;
// eza the user enters CTRL + D this will put buffer to NULL thats why we should clean up and exit;
// clean up and exit doest have anything specified yet, it should contain the struct and all memory allocations;

#include "minishell.h"

int	get_input(char *stash)
{
	char	*buffer;

	buffer = NULL;
	buffer = readline("minishell$ ");
	if (!buffer)
	{
		return(clean_up_and_exit());
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
// even if mafi space bayneton its still a token, example : <infile cmd|cmd <outfile  , should work perfectly fine;

static int	is_word_char(char c)
{
    return (c != ' ' && c != '|' && c != '<' && c != '>');
}

// im extracting the words to tokenize them later in the next function;
// pos is the index [i] of the next function;

char	*extract_word (char *input, int *pos)
{
	int 	i;
	int		len;
	int		in_single_quotes;
	int		in_double_quotes;

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
		else if (!in_single_quotes && !in_double_quotes && !is_word_char(input[*pos]))
			break ;
		(*pos)++;
		len++;
	}
	return (ft_substr(input, i, len));
}

// Now we got the extracted word from quotes;
// 3anna meshkelten;
// leading spaces and quotes preserved bel beginning and ending -> trim and clean

char	*clean_spaces(char *word)
{
	int	i[2];
	int	space;

	i[0] = 0;
	i[1] = 0;
	word = ft_strtrim(word, '\'');
	word = ft_strtrim(word, '\"');
	while (word[i[0]] == 32 || (word[i[0]] >= 9 && word[i[0]] <= 13))
        i[0]++;
	while (word[i[0]])
    {
		if (word[i[0]] == 32 || (word[i[0]] >= 9 && word[i[0]] <= 13))
			space = 1;
		else
		{
			if (space)
				word[i[1]++] = ' ';	
            space = 0;
			word[i[1]++] = word[i[0]];
		}
		i[0]++;
	}
	while (i[1] > 0 && word[i[1] - 1] == ' ')
        i[1]--;
    return (word[i[1]] = '\0', word);
}

t_token	*tokenize_input(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		while (input[i] == 32 || (input[i] >= 9 && input[i] <= 13))
			i++;
	}
}
