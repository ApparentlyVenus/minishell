/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_matches.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:19:58 by odana             #+#    #+#             */
/*   Updated: 2025/07/09 13:53:48 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * count_star_matches - Counts files matching the * pattern
 * 
 * @pattern: Pattern with * wildcards
 * @return: Number of matches found
 */
int	count_star_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue;
		if (match_star_pattern(pattern, entry->d_name))
			count++;
	}
	return (closedir(dir), count);
}

/*
 * fill_star_matches - Fills the matches array with matching filenames
 * 
 * - pattern: Pattern with * wildcards
 * - matches: Pre-allocated array to fill
 * - expected_count: Expected number of matches (for safety)
 * @return Actual number of matches filled
 */
int	fill_star_matches(const char *pattern, char **matches,
				int expected_count)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	while ((entry = readdir(dir)) != NULL && count < expected_count)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue;
		if (match_star_pattern(pattern, entry->d_name))
		{
			matches[count] = ft_strdup(entry->d_name);
			if (!matches[count])
			{
				while (--count >= 0)
					free(matches[count]);
				return (closedir(dir), 0);
			}
			count++;
		}
	}
	return (closedir(dir), count);
}

/*
 * collect_star_matches - Collects all files matching the * pattern
 * 
 * @pattern: Pattern with * wildcards
 * @matches: Pointer to array of matches (will be allocated)
 * @return: Number of matches found
 */
int	collect_star_matches(const char *pattern, char ***matches)
{
	int		count;
	char	**temp_matches;
	int		actual_count;

	count = count_star_matches(pattern);
	if (count == 0)
	{
		*matches = NULL;
		return (0);
	}
	temp_matches = (char **)malloc(sizeof(char *) * count);
	if (!temp_matches)
		return (0);
	actual_count = fill_star_matches(pattern, temp_matches, count);
	if (actual_count == 0)	
	{
		free(temp_matches);
		*matches = NULL;
		return (0);
	}
	*matches = temp_matches;
	return (actual_count);
}
