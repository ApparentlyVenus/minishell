/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:58:13 by odana             #+#    #+#             */
/*   Updated: 2025/07/13 19:03:53 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
** expand_variables_in_args
** Purpose: Expands variables in each argument string.
** Used variables: args (array), env (env list)
** Return: None (modifies args in place)
*/
void expand_variables_in_args(char **args, t_env *env)
{
	int i;
	char *expanded;

	i = 0;
	while (args && args[i])
	{
		expanded = expand_variables(args[i], env);
		if (expanded && expanded != args[i])
		{
			free(args[i]);
			args[i] = expanded;
		}
		i++;
	}
}

/*
** word_split_args
** Purpose: Splits arguments on unquoted whitespace and expands the array.
** Used variables: args (pointer to array)
** Return: None (modifies args in place, may reallocate array)
*/
void word_split_args(char ***args)
{
	int i;
	char **split;
	int j, k, count, new_count;
	char **new_args;

	i = 0;
	while (*args && (*args)[i])
	{
		if (contains_whitespace((*args)[i]))
		{
			split = ft_split((*args)[i], ' ');
			if (!split || !split[0])
			{
				free_split(split);
				i++;
				continue;
			}
			for (count = 0; (*args)[count]; count++);
			for (new_count = 0; split[new_count]; new_count++);
			new_args = (char **)malloc(sizeof(char *) * (count + new_count));
			j = 0;
			while (j < i)
			{
				new_args[j] = (*args)[j];
				j++;
			}
			k = 0;
			while (split[k])
				new_args[j++] = split[k++];
			k = i + 1;
			while ((*args)[k])
				new_args[j++] = (*args)[k++];
			new_args[j] = NULL;
			free((*args)[i]);
			free(*args);
			free(split);
			*args = new_args;
			i += new_count - 1;
		}
		i++;
	}
}

/*
** expand_wildcards_in_args
** Purpose: Expands wildcards in each argument, replacing with matches if any.
** Used variables: args (array, pointer to array)
** Return: None (modifies args in place, may reallocate array)
*/
void expand_wildcards_in_args(char ***args)
{
	int i;
	char **matches;
	int match_count;
	int new_count;
	char **new_args;
	int j, k;

	i = 0;
	while (*args && (*args)[i])
	{
		matches = NULL;
		match_count = wildcard_expand((*args)[i], &matches);
		if (match_count > 0)
		{
			// Count total new args
			for (new_count = 0; (*args)[new_count]; new_count++);
			new_args = (char **)malloc(sizeof(char *) * (new_count + match_count));
			j = 0;
			while (j < i)
			{
				new_args[j] = (*args)[j];
				j++;
			}
			k = 0;
			while (k < match_count)
				new_args[j++] = matches[k++];
			k = i + 1;
			while ((*args)[k])
				new_args[j++] = (*args)[k++];
			new_args[j] = NULL;
			free((*args)[i]);
			free(*args);
			*args = new_args;
			i += match_count - 1;
		}
		else if (matches)
		{
			free(matches);
		}
		i++;
	}
}

/*
** expand_all_args
** Purpose: Centralizes variable, word splitting, and wildcard expansion for args.
** Used variables: args (array, pointer to array), env (env list)
** Return: None (modifies args in place, may reallocate array)
*/
void expand_all_args(char ***args, t_env *env)
{
	expand_variables_in_args(*args, env);
	word_split_args(args);
	expand_wildcards_in_args(args);
} 