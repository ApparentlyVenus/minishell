/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helpers_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:38:10 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 00:06:55 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*trim_quotes(char *word, t_token *token)
{
	char	*trimmed;

	if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
	{
		token->single_quotes = 1;
		trimmed = ft_strtrim(word, "\'");
		if (!trimmed)
			return (word);
		free(word);
		return (trimmed);
	}
	else if (word[0] == '"' && word[ft_strlen(word) - 1] == '"')
	{
		token->double_quotes = 1;
		trimmed = ft_strtrim(word, "\"");
		if (!trimmed)
			return (word);
		free(word);
		return (trimmed);
	}
	return (word);
}

void	toggle_quotes(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '\"' && !(*in_single))
		*in_double = !(*in_double);
}

int	continue_word(char c, int in_single, int in_double)
{
	if (in_single || in_double)
		return (1);
	return (is_word_char(c) || c == '*');
}
