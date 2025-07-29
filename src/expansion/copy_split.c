/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:29:07 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 22:20:52 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_targs(t_arg **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

int	copy_targs_before(t_arg **new_args, t_arg **old_args, int pos)
{
	int	j;

	j = 0;
	while (j < pos)
	{
		new_args[j] = old_args[j];
		j++;
	}
	return (j);
}

int	copy_split_targs(t_arg **new_args, char **split, int j)
{
	int	k;

	k = 0;
	while (split[k])
	{
		new_args[j] = malloc(sizeof(t_arg));
		if (!new_args[j])
			return (j);
		new_args[j]->value = ft_strdup(split[k]);
		new_args[j]->single_quotes = 0;
		new_args[j]->double_quotes = 0;
		new_args[j]->next = NULL;
		j++;
		k++;
	}
	return (j);
}

int	copy_targs_after(t_arg **new_args, t_arg **old_args, int pos, int j)
{
	int	k;

	k = pos + 1;
	while (old_args[k])
	{
		new_args[j] = old_args[k];
		j++;
		k++;
	}
	new_args[j] = NULL;
	return (j);
}

int	copy_match_targs(t_arg **new_args, char **matches, int start_index,
	int match_count)
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
