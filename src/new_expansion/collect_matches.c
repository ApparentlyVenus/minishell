#include "../../inc/minishell.h"
#include "../../inc/new_expansion.h"

/*
** count_star_matches
** Purpose: Counts files matching the * pattern in the current directory.
** Used variables: pattern (wildcard)
** Return: Number of matches found
*/
int count_star_matches(const char *pattern)
{
	DIR *dir;
	struct dirent *entry;
	int count;

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
	closedir(dir);
	return (count);
}

/*
** fill_star_matches
** Purpose: Fills the matches array with matching filenames.
** Used variables: pattern (wildcard), matches (output array), expected_count
** Return: Actual number of matches filled
*/
int fill_star_matches(const char *pattern, char **matches, int expected_count)
{
	DIR *dir;
	struct dirent *entry;
	int count;

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
				closedir(dir);
				return (0);
			}
			count++;
		}
	}
	closedir(dir);
	return (count);
}

/*
** collect_star_matches
** Purpose: Collects all files matching the * pattern.
** Used variables: pattern (wildcard), matches (output array)
** Return: Number of matches found
*/
int collect_star_matches(const char *pattern, char ***matches)
{
	int count;
	char **temp_matches;
	int actual_count;

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