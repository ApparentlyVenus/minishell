/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:49:38 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 17:39:12 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	setup_new_arg_flags(t_arg *new_arg, t_arg **old_args,
	int *groups, int group_num)
{
	int	first_arg_idx;

	first_arg_idx = get_first_arg_of_group(old_args, groups, group_num);
	if (first_arg_idx >= 0)
	{
		new_arg->single_quotes = old_args[first_arg_idx]->single_quotes;
		new_arg->double_quotes = old_args[first_arg_idx]->double_quotes;
		new_arg->concat = old_args[first_arg_idx]->concat;
	}
	else
	{
		new_arg->single_quotes = 0;
		new_arg->double_quotes = 0;
		new_arg->concat = 0;
	}
	new_arg->next = NULL;
}

static int	create_new_args_array(t_arg **old_args, int *groups,
	int group_count, t_arg ***new_args)
{
	int		i;
	int		j;

	*new_args = malloc(sizeof(t_arg *) * (group_count + 1));
	if (!*new_args)
		return (0);
	j = 0;
	i = 0;
	while (i < group_count)
	{
		(*new_args)[j] = malloc(sizeof(t_arg));
		if (!(*new_args)[j])
			break ;
		(*new_args)[j]->value = concat_group_args(old_args, groups, i);
		setup_new_arg_flags((*new_args)[j], old_args, groups, i);
		i++;
		j++;
	}
	(*new_args)[j] = NULL;
	return (1);
}

void	concat_args(t_cmd *cmd)
{
	int		*groups;
	int		group_count;
	t_arg	**new_args;
	t_arg	**old_args;

	if (!cmd || !cmd->args)
		return ;
	find_adjacent_groups(cmd->args, &groups, &group_count);
	if (!groups)
		return ;
	old_args = cmd->args;
	if (!create_new_args_array(old_args, groups, group_count, &new_args))
	{
		free(groups);
		return ;
	}
	cmd->args = new_args;
	free(groups);
	free_old_args(old_args);
}
