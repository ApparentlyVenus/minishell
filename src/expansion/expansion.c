/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 10:21:18 by odana             #+#    #+#             */
/*   Updated: 2025/07/07 10:22:11 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Inside expansion.c

// ! This is a simplified example. Actual implementation would be more robust.

// * Assumes `get_env_value(char *key)` exists to get environment variable values.
// * Assumes `ft_split_by_ifs(char *str, const char *ifs)` exists (like ft_split, but using IFS).
// * Assumes `create_token_node(t_token_type type, char *value)` for new tokens.
// * Assumes functions to insert and delete nodes in the t_token linked list.

t_token	**expand_tokens(t_token **tokens)
{
    t_token *current = *tokens;
    t_token *prev = NULL;
    t_token *next_token;
    char    *expanded_value;
    char    **split_words;
    int     i;

    while (current)
    {
        next_token = current->next; // Store next token before potential current token modification/deletion

        if (current->type == TOKEN_WORD)
        {
            // 1. Tilde Expansion (if applicable, typically at start of word)
            // if (current->value[0] == '~' && !current->single_quotes && !current->double_quotes)
            //     current->value = expand_tilde(current->value); // This would re-allocate current->value

            // 2. Variable Expansion
            // This function needs to handle "c$VAR" -> "cat"
            // and "$VAR" (where VAR="one two") -> "one two"
            expanded_value = expand_variables(current->value);
            free(current->value); // Free the original value
            current->value = expanded_value;
            if (!current->value) // Handle potential memory allocation failure from expand_variables
            {
                // Error handling, cleanup and return NULL for the whole list
                // Or mark current token as an error and continue, depending on error strategy
            }

            // 3. Word Splitting (if not quoted)
            // This is ONLY applied if the original token was NOT inside double quotes
            // Single quotes inhibit variable expansion, so word splitting wouldn't apply after it.
            if (!current->double_quotes && !current->single_quotes) // Check original quote flags
            {
                split_words = ft_split_by_ifs(current->value, " \t\n"); // Using default IFS for now
                if (split_words)
                {
                    // If the split_words contains more than one element,
                    // we need to replace the current token with multiple new tokens.
                    if (split_words[0] && split_words[1]) // At least two words after splitting
                    {
                        // Replace current token with the first split word
                        free(current->value); // Free the value that was split
                        current->value = ft_strdup(split_words[0]);
                        // Ensure has_wildcard is reset for new, non-globbed words if applicable
                        current->has_wildcard = has_wildcard(current->value); // Re-evaluate wildcard flag

                        // Insert remaining split words as new tokens after the current one
                        t_token *last_inserted = current;
                        i = 1;
                        while (split_words[i])
                        {
                            t_token *new_word_token = create_token_node(TOKEN_WORD, ft_strdup(split_words[i]));
                            if (!new_word_token) {
                                // Handle error, free everything and return NULL
                                free_split(split_words);
                                return NULL; // Or suitable error handling
                            }
                            new_word_token->has_wildcard = has_wildcard(new_word_token->value); // Re-evaluate wildcard flag
                            new_word_token->next = last_inserted->next;
                            last_inserted->next = new_word_token;
                            last_inserted = new_word_token;
                            i++;
                        }
                        // Advance 'current' to the last inserted token so the loop continues from there
                        // correctly, or just set it to `last_inserted` which is already the end of the new chain.
                        current = last_inserted; // This is important to ensure `next_token` logic works.
                    }
                    else // Only one word after splitting (or empty string)
                    {
                        // If it resulted in an empty string (e.g., `echo $UNSET_VAR`)
                        // and it wasn't quoted, it might need to be removed.
                        // Bash behavior: `echo $UNSET_VAR` -> prints newline. `echo "$UNSET_VAR"` -> prints nothing.
                        // If it's empty and unquoted, the token might be removed.
                        if (ft_strlen(current->value) == 0 && !current->single_quotes && !current->double_quotes)
                        {
                            // Remove the current token from the list
                            if (prev)
                                prev->next = current->next;
                            else // It's the head
                                *tokens = current->next;
                            free_token_node(current); // Free the token and its value
                            current = prev; // Move current back to prev to correctly advance to next_token in loop
                        }
                    }
                    free_split(split_words); // Free the array of strings
                }
            }

            // 4. Wildcard Expansion (Globbing)
            // This is applied AFTER variable expansion and word splitting.
            if (current && current->has_wildcard && !current->single_quotes && !current->double_quotes)
            {
                // Calling expand_wildcards might return a new list of tokens
                // It will need to replace the current token in the linked list.
                // This function should return the new head of the expanded list of tokens
                // for this specific wildcard, and the number of new tokens.
                // For simplicity here, let's assume `expand_wildcards` handles insertion/deletion itself
                // and returns the last new token inserted, or current if no change.
                // Or, more simply, `expand_wildcards` takes `&current` and `&prev` to modify the list.
                // For now, let's just mark it as needing special handling and leave the complex list manipulation to `expand_wildcards`
                // `current = expand_wildcards_and_update_list(current, &prev, tokens);` // This is a complex function.
                // For now, let's just assume `expand_wildcards` modifies `current->value` and `current->type` if no matches,
                // or returns a new linked list segment.

                // Example: If `current->value` is "f*", `expand_wildcards` would search for files
                // If it finds "foo.txt", "bar.c", it needs to replace `current` token
                // with two new tokens for "foo.txt" and "bar.c".
                // This means `expand_tokens` itself needs to handle the linked list manipulation.

                // Re-evaluate current->value for globbing. If it still contains a wildcard
                // and was not quoted, then perform globbing.
                // This is the most complex part of expansion for list manipulation.
                // A helper function `insert_tokens_after(t_token *anchor, t_token *new_head_of_list)`
                // and `remove_token(t_token **head, t_token *target, t_token *prev_target)`
                // would be useful.
            }
        }
        prev = current;
        current = next_token;
    }
    return tokens;
}

// Pseudocode for expand_variables, which combines parts
char *expand_variables(char *value) {
    // This is where you'd build the new string.
    // It needs to handle sequences like "prefix$VARsuffix"
    // and correctly substitute.

    // Example logic:
    // Create a dynamic string (e.g., using a buffer or ft_strjoin iteratively)
    // Iterate through 'value':
    //   If '$' encountered:
    //     Extract variable name (e.g., "$VAR" -> "VAR")
    //     Get env value (e.g., `getenv("VAR")`)
    //     Append env value to new string (or empty string if not found)
    //     Skip past the variable in 'value'
    //   Else:
    //     Append character to new string
    //     Move to next character

    // This function should *not* perform word splitting.
    // It should just return the expanded single string.
    // e.g., "c$VAR" -> "cat"
    // e.g., "$HOME/file" -> "/home/user/file"
    // e.g., "foo $VAR bar" (where VAR="one two") -> "foo one two bar" (still a single string at this stage)

    // Example with no splitting:
    // char *new_str = ft_strdup(""); // Or a dynamic buffer
    // int i = 0;
    // while (value[i]) {
    //     if (value[i] == '$' && is_valid_var_start(value[i+1])) { // Implement is_valid_var_start
    //         char *var_name = extract_var_name(value, &i); // Extracts "VAR", updates i
    //         char *var_value = get_env_value(var_name); // Gets "at" or "one two"
    //         new_str = ft_strjoin_free_s1(new_str, var_value ? var_value : "");
    //         free(var_name);
    //     } else {
    //         new_str = ft_str_append_char_free_s1(new_str, value[i]); // Append character
    //         i++;
    //     }
    // }
    // return new_str;
    return ft_strdup(value); // Placeholder
}