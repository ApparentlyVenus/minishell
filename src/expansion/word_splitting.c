/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 22:19:50 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 22:20:07 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	insert_split_results(t_arg ***args, int index, char **split)
{
	t_arg	**new_args;
	int		old_count;
	int		split_count;
	int		j;

	old_count = count_targs(*args);
	split_count = count_args(split);
	new_args = malloc(sizeof(t_arg *) * (old_count + split_count));
	if (!new_args)
		return (0);
	j = copy_targs_before(new_args, *args, index);
	j = copy_split_targs(new_args, split, j);
	copy_targs_after(new_args, *args, index, j);
	free((*args)[index]->value);
	free((*args)[index]);
	free(*args);
	*args = new_args;
	return (split_count - 1);
}

int	perform_word_split(t_arg ***args, int index)
{
	char	**split;
	int		result;

	if (!(*args)[index]->value || !contains_whitespace((*args)[index]->value))
		return (0);
	split = ft_split((*args)[index]->value, ' ');
	if (!split || !split[0])
	{
		free_split(split);
		return (0);
	}
	result = insert_split_results(args, index, split);
	free_split(split);
	return (result);
}
