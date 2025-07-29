/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:21:37 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 22:34:57 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	insert_wildcard_matches(t_arg ***args, int index, char **matches,
		int match_count)
{
	t_arg	**new_args;
	int		old_count;
	int		j;

	old_count = count_targs(*args);
	new_args = malloc(sizeof(t_arg *) * (old_count + match_count));
	if (!new_args)
		return (0);
	j = copy_targs_before(new_args, *args, index);
	j = copy_match_targs(new_args, matches, j, match_count);
	copy_targs_after(new_args, *args, index, j);
	free((*args)[index]->value);
	free((*args)[index]);
	free(*args);
	*args = new_args;
	return (match_count - 1);
}

static int	insert_match_args(t_arg **new_args, char **matches,
		int start_index, int match_count)
{
	int	i;
	int	j;

	i = start_index;
	j = 0;
	while (j < match_count)
	{
		new_args[i] = malloc(sizeof(t_arg));
		if (!new_args[i])
			return (i);
		new_args[i]->value = ft_strdup(matches[j]);
		new_args[i]->single_quotes = 0;
		new_args[i]->double_quotes = 0;
		new_args[i]->next = NULL;
		i++;
		j++;
	}
	return (i);
}

int	perform_wildcard_expand(t_arg ***args, int index)
{
	char	**matches;
	int		match_count;
	int		result;

	if (!(*args)[index]->value || !has_wildcard((*args)[index]->value))
		return (0);
	match_count = expand_wildcard((*args)[index]->value, &matches);
	if (match_count <= 0)
	{
		if (matches)
			free(matches);
		return (0);
	}
	result = insert_wildcard_matches(args, index, matches, match_count);
	free_split(matches);
	return (result);
}
