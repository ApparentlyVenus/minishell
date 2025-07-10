/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:33:05 by odana             #+#    #+#             */
/*   Updated: 2025/07/10 15:16:31 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * sort_star_matches - Sorts the matched filenames alphabetically
 * 
 * @matches: Array of filename strings
 * @count: Number of matches
 */
void sort_star_matches(char **matches, int count)
{
	int     i;
    int     j;
	char	*temp;

    i = 0;
	while (i < count - 1)
	{
        j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(matches[j], matches[j + 1]) > 0)
			{
				temp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = temp;
			}
            j++;
		}
        i++;
	}
}

/*
 * create_star_tokens - Creates new tokens for * wildcard matches
 * 
 * - current: Current token containing
 * 
 * - matches: Array of matched filenames
 * 
 * - count: Number of matches
 * 
 * @return Last inserted token, or original token if no matches
 */
t_token *create_star_tokens(t_token *current, char **matches, int count)
{
	t_token *last;
	t_token *new_token;
	int i;

	if (count == 0)
		return (current);
	free(current->value);
	current->value = ft_strdup(matches[0]);
	current->has_wildcard = 0;
	last = current;
	i = 1;
	while (i < count)
	{
		new_token = init_star_token(matches[i]);
		new_token->next = last->next;
		last->next = new_token;
		last = new_token;
		i++;
	}
	return (last);
}
/*
 * init_star_token - Initalizes tokens created from wildcard expansion
 *
 * - match: String that matched the star pattern
 * 
 * @return new_token with initialized values
 */
t_token	*init_star_token(char *match)
{
	t_token	*new_token;
	
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(match);
	if (!new_token->value)
		return (NULL);
	new_token->priority = 0;
	new_token->single_quotes = 0;
	new_token->double_quotes = 0;
	new_token->has_wildcard = 0;
	new_token->type = TOKEN_WORD;
	return (new_token);
}

/*
 * expand_star_wildcard - Expands a single token containing * wildcards
 * 
 * - current: Token containing * pattern
 * 
 * @return Last inserted token after expansion, or original token if no matches
 */
t_token *expand_wildcard(t_token *current)
{
	char **matches;
	int count;
	t_token *result;
	int i;

	if (!current || !current->has_wildcard)
		return (current);

	count = collect_star_matches(current->value, &matches);
	
	if (count > 0)
	{
		sort_star_matches(matches, count);
		result = create_star_tokens(current, matches, count);
		i = 0;
		while (i < count)
			free(matches[i++]);
		free(matches);
		return (result);
	}
	if (matches)
		free(matches);
	return (current);
}
