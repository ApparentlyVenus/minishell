#include "../../inc/minishell.h"

/*
 * handle_empty_token - Removes empty tokens from the token list
 *
 * This function checks if a token is empty (zero length) and not quoted.
 * If so, it removes the token from the linked list and frees its memory.
 * This is necessary after variable expansion when a variable expands to nothing.
 *
 * @tokens: Pointer to the head of the token list
 * @current: The current token being processed
 * @prev: The previous token in the list (for linking)
 * @return: 1 if token was removed, 0 if token was kept
 */

int handle_empty_token(t_token **tokens, t_token *current, t_token *prev)
{
	if (ft_strlen(current->value) == 0 && !current->single_quotes && !current->double_quotes)
	{
		if (prev)
			prev->next = current->next;
		else
			*tokens = current->next;
		if (current->value)
			free(current->value);
		free(current);
		return (1);
	}
	return (0);
}

/*
 * init_new_token - Creates a new token and inserts it into the list
 *
 * This function creates a new token with the given word value and inserts it
 * right after the last_inserted token in the linked list. Used during word splitting.
 *
 * @word: The string value for the new token
 * @last_inserted: The token after which to insert the new token
 * @return: Pointer to the newly created token, or NULL on failure
 */
t_token *init_new_token(char *word, t_token *last_inserted)
{
	t_token *new_token;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(word);
	new_token->type = TOKEN_WORD;
	new_token->priority = 0;
	new_token->single_quotes = 0;
	new_token->double_quotes = 0;
	new_token->has_wildcard = has_wildcard(new_token->value);
	new_token->next = last_inserted->next;
	last_inserted->next = new_token;
	return (new_token);
}

/*
 * insert_split_words - Replaces current token with multiple split words
 *
 * This function takes a token and an array of split words, then replaces
 * the current token's value with the first word and creates new tokens
 * for the remaining words, inserting them in sequence.
 *
 * @current: The token to be replaced/expanded
 * @split_words: Array of strings from splitting the original token
 * @return: Pointer to the last inserted token, or NULL on failure
 */
t_token *insert_split_words(t_token *current, char **split_words)
{
	t_token *last_inserted;
	int i;
	free(current->value);
	current->value = ft_strdup(split_words[0]);
	current->has_wildcard = has_wildcard(current->value);
	last_inserted = current;
	i = 1;
	while (split_words[i])
	{
		last_inserted = init_new_token(split_words[i], last_inserted);
		if (!last_inserted)
			return (NULL);
		i++;
	}
	return (last_inserted);
}

/*
 * handle_word_splitting - Processes the result of word splitting
 *
 * This function handles different scenarios after attempting to split a word:
 * - If splitting produced multiple words: insert them into the token list
 * - If splitting produced no words: remove the empty token
 * - If splitting produced one word: keep the token as is
 *
 * @tokens: Pointer to the head of the token list
 * @current: The current token being processed
 * @prev: The previous token in the list
 * @split_words: Array of strings from splitting
 * @return: Split result code (see explanation below)
 */
int handle_word_splitting(t_token **tokens, t_token *current, t_token *prev, char **split_words)
{
	if (split_words[0] && split_words[1])
	{
		current = insert_split_words(current, split_words);
		if (!current)
		{
			free_split(split_words);
			return (-1);
		}
		return (1);
	}
	else if (handle_empty_token(tokens, current, prev))
		return (2);
	return (0);
}

/*
 * apply_word_splitting - Attempts to split a token on whitespace
 *
 * This function checks if a token should be split (not quoted and contains whitespace),
 * then performs the splitting and handles the results.
 *
 * @tokens: Pointer to the head of the token list
 * @current: The current token being processed
 * @prev: The previous token in the list
 * @return: Split result code (see explanation below)
 */
int apply_word_splitting(t_token **tokens, t_token *current, t_token *prev)
{
	char **split_words;
	int result;
	if (current->double_quotes || current->single_quotes)
		return (NO_CHANGE);
	if (!contains_whitespace(current->value))
		return (NO_CHANGE);
	split_words = ft_split(current->value, ' ');
	if (!split_words)
		return (NO_CHANGE);
	result = handle_word_splitting(tokens, current, prev, split_words);
	free_split(split_words);
	return (result);
} 