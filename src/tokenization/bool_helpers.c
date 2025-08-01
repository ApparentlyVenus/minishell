/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bool_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:56:54 by yitani            #+#    #+#             */
/*   Updated: 2025/08/01 15:03:37 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_word_char(char c)
{
	return (c != ' ' && c != '|' && c != '<'
		&& c != '>' && c != '&' && c != '(' && c != ')'
		&& c != '\t' && c != '\n' && c != '\r');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '&' || c == ')' || c == '(');
}

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

int	has_wildcard(char *word)
{
	int	i;

	if (!word)
		return (0);
	i = 0;
	while (word[i])
	{
		if (word[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

int	is_closed(char *input, int pos)
{
	int		i;
	char	quote_char;
	int		found_close;

	i = pos;
	found_close = 0;
	if (!input[i] || !is_quotes(input[i]))
		return (1);
	quote_char = input[i];
	i++;
	while (input[i])
	{
		if (input[i] == quote_char)
		{
			found_close = 1;
			break ;
		}
		i++;
	}
	return (found_close);
}
