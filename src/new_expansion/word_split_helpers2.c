/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split_helpers2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:29:07 by odana             #+#    #+#             */
/*   Updated: 2025/07/18 13:34:07 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** count_args
** Purpose: Counts the number of arguments in array
** Used variables: args (array)
** Return: Number of arguments
*/
int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

/*
** copy_args_before
** Purpose: Copies arguments before the split position
** Used variables: new_args, old_args, pos
** Return: Index where to continue copying
*/
int	copy_args_before(char **new_args, char **old_args, int pos)
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

/*
** copy_split_args
** Purpose: Copies the split arguments into new array
** Used variables: new_args, split, j
** Return: Updated index
*/
int	copy_split_args(char **new_args, char **split, int j)
{
	int	k;

	k = 0;
	while (split[k])
		new_args[j++] = split[k++];
	return (j);
}

/*
** copy_args_after
** Purpose: Copies arguments after the split position
** Used variables: new_args, old_args, pos, j
** Return: Updated index
*/
int	copy_args_after(char **new_args, char **old_args, int pos, int j)
{
	int	k;

	k = pos + 1;
	while (old_args[k])
		new_args[j++] = old_args[k++];
	return (j);
}
