/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:36:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 17:37:38 by yitani           ###   ########.fr       */
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
