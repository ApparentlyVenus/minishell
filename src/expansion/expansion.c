/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 10:21:18 by odana             #+#    #+#             */
/*   Updated: 2025/07/08 11:20:24 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
static int handle_empty_token(t_token **tokens, t_token *current, t_token *prev)
{
	// Check if token is empty AND not quoted (empty quotes should be preserved)
	if (ft_strlen(current->value) == 0 && !current->single_quotes && !current->double_quotes)
	{
		// Relink the list: skip over the current token
		if (prev)
			prev->next = current->next; // Previous token points to next
		else
			*tokens = current->next; // Update head if removing first token

		// Free the memory allocated for this token
		if (current->value)
			free(current->value);
		free(current);
		return (1); // Token was removed
	}
	return (0); // Token was kept
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
static t_token *init_new_token(char *word, t_token *last_inserted)
{
	t_token *new_token;

	// Allocate memory for the new token structure
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);

	// Initialize the new token with default values
	new_token->value = ft_strdup(word);						  // Copy the word value
	new_token->type = TOKEN_WORD;							  // Set as word token
	new_token->priority = 0;								  // Default priority
	new_token->single_quotes = 0;							  // Not single-quoted
	new_token->double_quotes = 0;							  // Not double-quoted
	new_token->has_wildcard = has_wildcard(new_token->value); // Check for wildcards

	// Insert into the linked list after last_inserted
	new_token->next = last_inserted->next; // Point to what last_inserted pointed to
	last_inserted->next = new_token;	   // Make last_inserted point to new token

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
static t_token *insert_split_words(t_token *current, char **split_words)
{
	t_token *last_inserted;
	int i;

	// Replace current token's value with first split word
	free(current->value);								  // Free old value
	current->value = ft_strdup(split_words[0]);			  // Set to first split word
	current->has_wildcard = has_wildcard(current->value); // Update wildcard flag

	last_inserted = current; // Start from current token
	i = 1;					 // Start from second split word

	// Create new tokens for remaining split words
	while (split_words[i])
	{
		last_inserted = init_new_token(split_words[i], last_inserted);
		if (!last_inserted) // Check for allocation failure
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
static int handle_word_splitting(t_token **tokens, t_token *current,
								 t_token *prev, char **split_words)
{
	// Check if we have multiple words after splitting
	if (split_words[0] && split_words[1])
	{
		// Insert all split words into the token list
		current = insert_split_words(current, split_words);
		if (!current)
		{
			free_split(split_words);
			return (-1); // Error occurred during insertion
		}
		return (1); // Successfully split into multiple tokens
	}
	// Check if splitting resulted in an empty token
	else if (handle_empty_token(tokens, current, prev))
		return (2); // Token was removed

	return (0); // No splitting occurred (single word or no change)
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
static int apply_word_splitting(t_token **tokens, t_token *current,
								t_token *prev)
{
	char **split_words;
	int result;

	// Don't split quoted tokens (preserve spaces in quotes)
	if (current->double_quotes || current->single_quotes)
		return (NO_CHANGE);
	if (!contains_whitespace(current->value))
		return (NO_CHANGE);
	split_words = ft_split(current->value, ' ');
	if (!split_words)
		return (NO_CHANGE); // Allocation failure

	// Handle the splitting results
	result = handle_word_splitting(tokens, current, prev, split_words);
	free_split(split_words); // Clean up the split array
	return (result);
}

/*
 * expand_variables - Expands environment variables in a token
 *
 * This function takes a token and expands any environment variables
 * (like $HOME, $PATH) found in its value, replacing the token's value
 * with the expanded version.
 *
 * @current: The token whose value should be expanded
 * @return: 1 on success, 0 on failure
 */
static int expand_variables(t_token *current)
{
	char *expanded_value;

	// Expand variables in the token value
	expanded_value = expand_variables(current->value);
	if (!expanded_value)
		return (0); // Expansion failed

	// Replace the old value with the expanded one
	free(current->value);
	current->value = expanded_value;
	return (1); // Success
}

/*
 * process_word_token - Processes a single word token through all expansions
 *
 * This function applies the complete expansion process to a word token:
 * 1. Variable expansion (replace $VAR with values)
 * 2. Word splitting (split on whitespace if unquoted)
 * 3. Wildcard expansion (expand * and ? if unquoted)
 *
 * @tokens: Pointer to the head of the token list
 * @current: The current token being processed
 * @prev: The previous token in the list
 * @return: Pointer to the next token to process, or NULL on error
 */
static t_token *process_word_token(t_token **tokens, t_token *current,
								   t_token *prev)
{
	int split_result;

	// Step 1: Expand environment variables
	if (!expand_variables(current))
		return (NULL); // Variable expansion failed

	// Step 2: Apply word splitting
	split_result = apply_word_splitting(tokens, current, prev);
	if (split_result == ERROR)
		return (NULL); // Error during splitting
	if (split_result == TOKEN_REMOVED)
		return (prev); // Current token was removed, continue from previous
	if (split_result == SPLIT_SUCCESS)
		return (current); // Token was split, continue from current (last inserted)

	// Step 3: Apply wildcard expansion (only if not quoted)
	if (current && current->has_wildcard && !current->single_quotes && !current->double_quotes)
	{
		current = expand_wildcards_in_place(current);
	}

	return (current); // Return the processed token
}

/*
 * expand_tokens - Main function to expand all tokens in the list
 *
 * This is the main entry point for token expansion. It iterates through
 * all tokens in the list and processes each TOKEN_WORD through the
 * complete expansion pipeline.
 *
 * @tokens: Pointer to the head of the token list
 * @return: Pointer to the token list, or NULL on error
 */
t_token	**expand_tokens(t_token **tokens)
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
			current = process_word_token(tokens, current, prev);
			if (!current)
				return (NULL);
		}
		prev = current;
		current = next_token;
	}
	return (tokens);
}

/* ************************************************************************** */
/*                          Variable Expansion Functions                      */
/* ************************************************************************** */

/*
 * extract_var_name - Extracts a variable name from a string
 *
 * Starting from the current position, this function extracts a complete
 * variable name and advances the index past it.
 *
 * @value: The string containing the variable name
 * @i: Pointer to current position (will be updated)
 * @return: Allocated string containing the variable name, or NULL on error
 */
static char	*extract_var_name(char *value, int *i)
{
	int		start;
	int		len;
	char	*var_name;

	start = *i; // Remember where the variable name starts

	// Skip through all valid variable characters
	while (value[*i] && is_valid_var_char(value[*i]))
		(*i)++;

	// Calculate the length of the variable name
	len = *i - start;

	// Allocate memory for the variable name
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (NULL);

	// Copy the variable name and null-terminate it
	ft_strncpy(var_name, value + start, len);
	var_name[len] = '\0';

	return (var_name);
}

/*
 * append_str - Appends a single character to a string
 *
 * This function creates a new string that is one character longer
 * than the original, copies the original content, and appends the new character.
 * The original string is freed.
 *
 * @str: The original string (will be freed)
 * @c: The character to append
 * @return: New string with character appended, or NULL on error
 */
static char	*append_str(char *str, char c)
{
	char	*new_str;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	// Allocate space for original string + new character + null terminator
	new_str = malloc(len + 2);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	// Copy the original string
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	// Append the new character and null terminator
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str); // Free the original string
	return (new_str);
}

/*
 * join_and_free_first - Joins two strings and frees the first one
 *
 * This is a utility function that joins two strings and automatically
 * frees the first string to prevent memory leaks.
 *
 * @s1: First string (will be freed)
 * @s2: Second string (will not be freed)
 * @return: Joined string, or s1 if s2 is NULL
 */
static char	*join_and_free_first(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
		return (s1);
	result = ft_strjoin(s1, s2); // Join the strings
	free(s1);			 // Free the first string
	return (result);
}

/*
 * expand_variables - Expands environment variables in a string
 *
 * This function processes a string and replaces all environment variable
 * references (like $HOME, $PATH) with their actual values from the environment.
 *
 * NOTE: There appears to be a bug in the original code - it checks for
 * value[i] == ' ' instead of value[i] == '$' when looking for variables.
 *
 * @value: The string containing variable references
 * @return: New string with variables expanded, or NULL on error
 */
char	*expand_variables(char *value, t_env *env)
{
	char	*new_str;
	char	*var_name;
	char	*var_value;
	int		i;

	if (!contains_dollar(value))
		return (ft_strdup(value));
	new_str = ft_strdup("");
	if (!new_str)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && is_valid_var_start(value[i + 1]))
		{
			i++;
			var_name = extract_var_name(value, &i);
			if (!var_name)
				return (free(new_str), NULL);
			var_value = get_env_value(env, var_name);
			new_str = join_and_free_first(new_str, var_value ? var_value : "");
			free(var_name);
		}
		else
			new_str = append_char_to_str(new_str, value[i++]);
		if (!new_str)
			return (NULL);
	}
	return (new_str);
}

/*
 * EXPLANATION OF SPLIT RESULT RETURN VALUES:
 *
 * The split_result variable and its return values represent different outcomes
 * of the word splitting process:
 *
 * -1 = ERROR: Memory allocation failed during splitting
 *      Action: Return NULL to indicate failure
 *
 *  0 = NO_CHANGE: No splitting occurred
 *      Reasons: Token was quoted, contained no whitespace, or splitting failed
 *      Action: Continue processing normally, move to next token
 *
 *  1 = SPLIT_SUCCESS: Token was successfully split into multiple tokens
 *      Action: Continue from the current token (which is now the last inserted token)
 *      The iteration should continue from this position since new tokens were inserted
 *
 *  2 = TOKEN_REMOVED: The token was empty after expansion and was removed
 *      Action: Continue from the previous token since current token no longer exists
 *      The iteration needs to backtrack to avoid skipping the next token
 *
 * This return code system allows the main processing loop to handle different
 * scenarios appropriately and maintain correct iteration through the token list
 * even when the list structure changes during processing.
 */
