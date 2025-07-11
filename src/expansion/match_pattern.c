/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_pattern.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:00:10 by odana             #+#    #+#             */
/*   Updated: 2025/07/11 15:53:11 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * handle_star_case - Handles when we encounter a * in the pattern
 * 
 * @p: Pointer to current position in pattern (will be modified)
 * @f: Current position in filename
 * @star_p: Pointer to store position after *
 * @star_f: Pointer to store filename position when * was found
 * @return: 1 if pattern ends with *, 0 to continue matching
 */
int	handle_star_case(const char **p, const char *f,
				const char **star_p, const char **star_f)
{
	while (**p == '*')
		(*p)++;
	if (!**p)
		return (1);
	*star_p = *p;
	*star_f = f;
	return (0);
}

/*
 * try_backtrack - Attempts to backtrack to the last * position
 * 
 * @p: Pointer to current position in pattern
 * @f: Pointer to current position in filename
 * @star_p: Position in pattern after the last *
 * @star_f: Pointer to position in filename when * was found (will be modified)
 * @return: 1 if backtrack possible, 0 if not
 */
int	try_backtrack(const char **p, const char **f,
				const char *star_p, const char **star_f)
{
	if (!star_p)
		return (0);
	*p = star_p;
	*f = ++(*star_f);
	return (1);
}

/*
 * match_star_pattern - Checks if a filename matches a pattern with * wildcards
 * 
 * @pattern: Pattern containing * wildcards
 * @filename: Filename to match against
 * @return: 1 if match, 0 if no match
 */
int	match_star_pattern(const char *pattern, const char *filename)
{
	const char	*p = pattern;
	const char	*f = filename;
	const char	*star_p = NULL;
	const char	*star_f = NULL;

	while (*f)
	{
		if (*p == '*')
		{
			if (handle_star_case(&p, f, &star_p, &star_f))
				return (1);
		}
		else if (*p == *f)
		{
			p++;
			f++;
		}
		else if (!try_backtrack(&p, &f, star_p, &star_f))
			return (0);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}
