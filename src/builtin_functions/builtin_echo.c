/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 03:46:17 by yitani            #+#    #+#             */
/*   Updated: 2025/07/16 23:41:53 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	new_line_flag(char **args, int *i)
{
	int	j;

	*i = 0;
	j = 0;
	while (args[j] && ft_strcmp(args[j], "-n") == 0)
	{
		*i = 1;
		j++;
	}
	return (j);
}

int	builtin_echo(char **args)
{
	int	i;
	int	j;

	if (args[0])
	{
		j = new_line_flag(args, &i);
		while (args[j])
		{
			if (i == 1 && ft_strcmp(args[j], "\n") == 0)
			{
				j++;
				continue ;
			}
			write(1, args[j], ft_strlen(args[j]));
			if (args[j + 1] != NULL)
				write(1, " ", 1);
		}
		j++;
	}
	if (i == 0)
		write(1, "\n", 1);
	return (0);
}
