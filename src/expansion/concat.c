/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:49:38 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 02:38:35 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	find_adjacent_groups(t_arg **args, int **groups, int *group_count)
{
	int	i;
	int	current_group;
	int	arg_count;

	arg_count = count_targs(args);
	*groups = malloc(sizeof(int) * arg_count);
	if (!*groups)
		return ;
	current_group = 0;
	i = 0;
	while (args[i])
	{
		(*groups)[i] = current_group;
		if (args[i + 1] && args[i + 1]->concat == 0)
			current_group++;
		i++;
	}
	*group_count = current_group + 1;
}

int	get_first_arg_of_group(t_arg **args, int *groups, int group_num)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (groups[i] == group_num)
			return (i);
		i++;
	}
	return (-1);
}

char	*concat_group_args(t_arg **args, int *groups, int group_num)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (args[i])
	{
		if (groups[i] == group_num)
		{
			temp = ft_strjoin(result, args[i]->value);
			free(result);
			result = temp;
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}

void	free_old_args(t_arg **old_args)
{
	int	i;

	if (!old_args)
		return ;
	i = 0;
	while (old_args[i])
	{
		if (old_args[i]->value)
			free(old_args[i]->value);
		free(old_args[i]);
		i++;
	}
	free(old_args);
}

void	concat_args(t_cmd *cmd)
{
	int		*groups;
	int		group_count;
	t_arg	**new_args;
	t_arg	**old_args;
	int		first_arg_idx;
	int		i;
	int		j;

	if (!cmd || !cmd->args)
		return ;
	find_adjacent_groups(cmd->args, &groups, &group_count);
	if (!groups)
		return ;
	new_args = malloc(sizeof(t_arg *) * (group_count + 1));
	if (!new_args)
		return (free(groups));
	old_args = cmd->args;
	j = 0;
	i = 0;
	while (i < group_count)
	{
		new_args[j] = malloc(sizeof(t_arg));
		if (!new_args[j])
			break ;
		new_args[j]->value = concat_group_args(old_args, groups, i);
		first_arg_idx = get_first_arg_of_group(old_args, groups, i);
		if (first_arg_idx >= 0)
		{
			new_args[j]->single_quotes = old_args[first_arg_idx]->single_quotes;
			new_args[j]->double_quotes = old_args[first_arg_idx]->double_quotes;
			new_args[j]->concat = old_args[first_arg_idx]->concat;
		}
		else
		{
			new_args[j]->single_quotes = 0;
			new_args[j]->double_quotes = 0;
			new_args[j]->concat = 0;
		}
		new_args[j]->next = NULL;
		i++;
		j++;
	}
	new_args[j] = NULL;
	cmd->args = new_args;
	free(groups);
	free_old_args(old_args);
}
