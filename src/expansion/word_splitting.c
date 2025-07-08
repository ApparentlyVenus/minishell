#include "../../inc/minishell.h"


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