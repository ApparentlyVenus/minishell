/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:02:46 by odana             #+#    #+#             */
/*   Updated: 2025/07/18 13:34:12 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** contains_whitespace
** Purpose: Checks if a string contains any whitespace character.
** Used variables: str (input string)
** Return: 1 if whitespace found, 0 otherwise
*/
int	contains_whitespace(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/*
** free_split
** Purpose: Frees a NULL-terminated array of strings.
** Used variables: args (array)
** Return: None
*/
void	free_split(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

/*
** create_new_args_array
** Purpose: Creates new argument array with split results
** Used variables: args, split, pos, old_count, new_count
** Return: New argument array
*/
char	**create_new_args_array(char ***args, char **split, int pos)
{
	char	**new_args;
	int		old_count;
	int		new_count;
	int		j;

	old_count = count_args(*args);
	new_count = count_args(split);
	new_args = malloc(sizeof(char *) * (old_count + new_count));
	if (!new_args)
		return (NULL);
	j = copy_args_before(new_args, *args, pos);
	j = copy_split_args(new_args, split, j);
	j = copy_args_after(new_args, *args, pos, j);
	new_args[j] = NULL;
	return (new_args);
}

/*
** replace_args_with_split
** Purpose: Replaces argument at position with split results
** Used variables: args, split, i
** Return: Number of new arguments added
*/
int	replace_args_with_split(char ***args, char **split, int i)
{
	char	**new_args;
	int		new_count;

	new_args = create_new_args_array(args, split, i);
	if (!new_args)
		return (0);
	new_count = count_args(split);
	free((*args)[i]);
	free(*args);
	free(split);
	*args = new_args;
	return (new_count - 1);
}
