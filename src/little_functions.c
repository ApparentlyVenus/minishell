/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:56:54 by yitani            #+#    #+#             */
/*   Updated: 2025/06/28 20:28:17 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_word_char(char c)
{
    return (c != ' ' && c != '|' && c != '<' && c != '>');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_closed(char *input)
{
	int		i;
	char	q;
	int		is_word;

	is_word = 0;
	i = 0;
	if (input[i] == '\'' || input[i] == '\"')
	{
		q = input[i];
		while (input[i])
		{
			i++;
			if (input[i] == q)
			{
				is_word = 1;
				return (1);
			}
		}
	}
	return (0);
}
