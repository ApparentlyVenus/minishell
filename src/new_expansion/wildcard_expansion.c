/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:04 by odana             #+#    #+#             */
/*   Updated: 2025/07/24 23:30:49 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
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

int	match_star_pattern(const char *pattern, const char *filename)
{
	const char	*star_p = NULL;
	const char	*star_f = NULL;

	while (*filename)
	{
		if (*pattern == '*')
		{
			star_p = ++pattern;
			star_f = filename;
		}
		else if (*pattern == *filename && (pattern++, filename++, 1))
			;
		else if (star_p)
		{
			pattern = star_p;
			filename = ++star_f;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern);
}

int	count_star_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_star_pattern(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	return (closedir(dir), count);
}

int	wildcard_expand(char *pattern, char ***matches)
{
	int	count;

	count = collect_star_matches(pattern, matches);
	if (count > 1)
		sort_matches(*matches, count);
	return (count);
}
