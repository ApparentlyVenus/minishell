#include "../../inc/new_expansion.h"
#include "../../inc/minishell.h"

/*
** wildcard_expand
** Purpose: Uses collect_star_matches and sort_star_matches to expand a wildcard pattern.
** Used variables: pattern (wildcard), matches (output array)
** Return: Number of matches found, -1 on error
*/
int wildcard_expand(char *pattern, char ***matches)
{
	int count;

	count = collect_star_matches(pattern, matches);
	if (count > 1)
		sort_star_matches(*matches, count);
	return (count);
}

/*
** sort_matches
** Purpose: Alias for sort_star_matches for new expansion interface.
** Used variables: matches (array), count (number of strings)
** Return: None (modifies matches in place)
*/
void sort_matches(char **matches, int count)
{
	sort_star_matches(matches, count);
}
