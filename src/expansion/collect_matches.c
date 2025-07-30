/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_matches.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:09 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 00:10:29 by yitani           ###   ########.fr       */
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
	if (ft_strcmp(entry->d_name, ".") == 0
		|| ft_strcmp(entry->d_name, "..") == 0)
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

	if (!pattern || !matches)
		return (0);
	count = count_star_matches(pattern);
	if (count == 0)
		return (*matches = NULL, 0);
	temp_matches = (char **)malloc(sizeof(char *) * (count + 1));
	if (!temp_matches)
		return (0);
	actual_count = 0;
	while (actual_count <= count)
	{
		temp_matches[actual_count] = NULL;
		actual_count++;
	}
	actual_count = fill_star_matches(pattern, temp_matches, count);
	if (actual_count == 0)
		return (free(temp_matches), *matches = NULL, (0));
	return (*matches = temp_matches,
		temp_matches[actual_count] = NULL, actual_count);
}
