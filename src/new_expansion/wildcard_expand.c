/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:15 by odana             #+#    #+#             */
/*   Updated: 2025/07/13 19:04:01 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		sort_matches(*matches, count);
	return (count);
}
