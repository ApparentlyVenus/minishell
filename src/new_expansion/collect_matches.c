/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_matches.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:09 by odana             #+#    #+#             */
/*   Updated: 2025/07/24 21:50:51 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cleanup_matches_on_error(char **matches, int count)
{
	while (--count >= 0)
		free(matches[count]);
}

int	should_skip_entry(struct dirent *entry, const char *pattern)
{
	if (entry->d_name[0] == '.' && pattern[0] != '.')
		return (1);
	return (0);
}

int	add_match_to_array(char **matches, int count, char *filename)
{
	matches[count] = ft_strdup(filename);
	if (!matches[count])
	{
		cleanup_matches_on_error(matches, count);
		return (0);
	}
	return (1);
}

int	fill_star_matches(const char *pattern, char **matches, int expected_count)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry != NULL && count < expected_count)
	{
		if (should_skip_entry(entry, pattern))
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_star_pattern(pattern, entry->d_name))
		{
			if (!add_match_to_array(matches, count, entry->d_name))
				return (closedir(dir), 0);
			count++;
		}
		entry = readdir(dir);
	}
	return (closedir(dir), count);
}

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
