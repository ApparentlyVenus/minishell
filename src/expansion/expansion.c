/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 10:21:18 by odana             #+#    #+#             */
/*   Updated: 2025/07/11 16:11:03 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * process_word_token - Processes a single word token through all expansions
 *
 * This function applies the complete expansion process to a word token:
 * 
 * 1. Variable expansion (replace $VAR with values)
 * 
 * 2. Word splitting (split on whitespace if unquoted)
 * 
 * 3. Wildcard expansion (expand *)
 *
 * - tokens: Pointer to the head of the token list
 * 
 * - current: The current token being processed
 * 
 * - prev: The previous token in the list
 * 
 * @return Pointer to the next token to process, or NULL on error
 */
t_token *process_word_token(t_token **tokens, t_token *current,
								   t_token *prev, t_env *env)
{
	int		split_result;
	char	*expanded;

	expanded = expand_variables(current->value, env);
	if (!expanded)
		return (NULL);
	free(current->value);
	current->value = expanded;
	split_result = apply_word_splitting(tokens, current, prev);
	if (split_result == ERROR)
		return (NULL);
	if (split_result == TOKEN_REMOVED)
		return (prev);
	if (split_result == SPLIT_SUCCESS)
		return (current);
	if (current && current->has_wildcard && !current->single_quotes && !current->double_quotes)
		current = expand_wildcard(current);
	return (current);
}

/*
 * expand_tokens - Main function to expand all tokens in the list
 *
 * This is the main entry point for token expansion. It iterates through
 * all tokens in the list and processes each TOKEN_WORD through the
 * complete expansion pipeline.
 *
 * - tokens: Pointer to the head of the token list
 * 
 * @return Pointer to the token list, or NULL on error
 */
t_token	**expand_tokens(t_token **tokens, t_env *env)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next_token;

	current = *tokens;
	prev = NULL;
	while (current)
	{
		next_token = current->next;
		if (current->type == TOKEN_WORD)
		{
			current = process_word_token(tokens, current, prev, env);
			if (!current)
				return (NULL);
		}
		prev = current;
		current = next_token;
	}
	return (tokens);
}
