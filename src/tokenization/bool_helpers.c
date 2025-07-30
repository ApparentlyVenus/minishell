/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bool_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:56:54 by yitani            #+#    #+#             */
/*   Updated: 2025/07/30 23:05:24 by odana            ###   ########.fr       */
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
	char	q;

	i = pos;
	if (!input[i])
		return (0);
	if (input[i] == '\'' || input[i] == '\"')
	{
		q = input[i];
		i++;
		while (input[i])
		{
			i++;
			if (input[i] == q)
				return (1);
		}
	}
	return (0);
}
