/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 00:43:43 by yitani            #+#    #+#             */
/*   Updated: 2025/07/03 00:21:31 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*handle_any_word(char *input, char *word, int *i, t_token *new_token)
{
	if (is_quotes(input[*i]) && is_closed(input, *i))
	{
		word = extract_word(input, *i);
		new_token = clean_word_token(word);
	}
	else if (is_quotes(input[*i]) && !(is_closed(input, *i)))
		return (NULL);
	else if (is_word_char(input[*i]))
	{
		word = extract_word(input, &i);
		new_token = clean_word_token(word);
	}
	return (new_token);
}

void	free_tokens(t_token **head)
{
	t_token	*current;
	t_token	*next;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*head = (NULL);
}
