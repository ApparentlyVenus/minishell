/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 07:13:54 by yitani            #+#    #+#             */
/*   Updated: 2025/07/11 10:24:16 by odana            ###   ########.fr       */
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
		return (set_error(shell, "minishell: malloc failure"), NULL);
	if (is_quotes(input[start]) && !is_closed(input, start))
		return (free(word), set_error(shell, "minishell: unclose quotes"), NULL);
	new_token = clean_word_token(word);
	if (!new_token)
		return (set_error(shell, "minishehll: malloc failure"), NULL);
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
		return (set_error(shell, "minishell: unexpected character"), NULL);
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

void	skip_spaces(char *input, int *pos)
{
	while (input[*pos] == 32 || (input[*pos] >= 9 && input[*pos] <= 13))
		(*pos)++;
}
