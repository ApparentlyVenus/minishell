/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:04 by odana             #+#    #+#             */
/*   Updated: 2025/07/13 18:58:05 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** sort_matches
** Purpose: Sorts the matched filenames alphabetically.
** Used variables: matches (array), count (number of matches)
** Return: None (modifies matches in place)
*/
void sort_matches(char **matches, int count)
{
	int i;
	int j;
	char *temp;

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
** match_star_pattern
** Purpose: Checks if a filename matches a wildcard pattern.
** Used variables: pattern (wildcard), filename (candidate)
** Return: 1 if match, 0 otherwise
*/
int match_star_pattern(const char *pattern, const char *filename)
{
	int i;
	int j;
	char c;

	i = 0;
	j = 0;
	while (pattern[i] && filename[j])
	{
		c = pattern[i];
		if (c == '*')
		{
			if (!pattern[i + 1])
				return (1);
			while (filename[j] && filename[j] != pattern[i + 1])
				j++;
			i++;
		}
		else if (c == filename[j])
		{
			i++;
			j++;
		}
		else
			return (0);
	}
	while (pattern[i] == '*')
		i++;
	return (!pattern[i] && !filename[j]);
} 