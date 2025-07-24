/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:13:54 by yitani            #+#    #+#             */
/*   Updated: 2025/07/25 02:09:15 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*handle_word_token(char *input, int *i, t_shell *shell)
{
	char	*word;
	t_token	*new_token;
	int		start;

	start = *i;
	word = extract_word(input, i);
	if (!word)
		return (handle_error(shell, "malloc failure",
				EXIT_GENERAL_ERROR), NULL);
	if (is_quotes(input[start]) && !is_closed(input, start))
		return (free(word), handle_error(shell, "unclose quotes",
				EXIT_MISUSE), NULL);
	new_token = clean_word_token(word);
	if (!new_token)
		return (handle_error(shell, "malloc failure",
				EXIT_GENERAL_ERROR), NULL);
	return (new_token);
}

t_token	*create_next_token(char *input, int *i, t_shell *shell)
{
	t_token	*new_token;

	new_token = NULL;
	if (is_operator(input[*i]))
		new_token = extract_operator_token(input, i);
	else if (is_quotes(input[*i]) || is_word_char(input[*i]))
		new_token = handle_word_token(input, i, shell);
	else
		return (handle_error(shell, "unexpected character", EXIT_MISUSE), NULL);
	return (new_token);
}

void	free_tokens(t_token **head)
{
	t_token	*current;
	t_token	*next;
	int		count;

	count = 0;
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
		count++;
	}
	*head = NULL;
}

void	skip_spaces(char *input, int *pos)
{
	while (input[*pos] == 32 || (input[*pos] >= 9 && input[*pos] <= 13))
		(*pos)++;
}
