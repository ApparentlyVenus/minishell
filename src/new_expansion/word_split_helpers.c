/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:02:46 by odana             #+#    #+#             */
/*   Updated: 2025/07/13 19:02:47 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** contains_whitespace
** Purpose: Checks if a string contains any whitespace character.
** Used variables: str (input string)
** Return: 1 if whitespace found, 0 otherwise
*/
int contains_whitespace(char *str)
{
	int i;

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
void free_split(char **args)
{
	int i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
} 