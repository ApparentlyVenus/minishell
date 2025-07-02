/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:56:54 by yitani            #+#    #+#             */
/*   Updated: 2025/07/02 04:13:07 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_word_char(char c)
{
    return (c != ' ' && c != '|' && c != '<' &&
		c != '>' && c != '&');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

void	skip_spaces(char *input, int *pos)
{
	while (input[*pos] == 32 || (input[*pos] >= 9 && input[*pos] <= 13))
		(*pos)++;
}

int	is_closed(char *input , int pos)
{
	int		i;
	char	q;
	int		is_word;

	is_word = 0;
	i = pos;
	if (!input[i])
		return (0);
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
