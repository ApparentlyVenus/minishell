/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 03:46:17 by yitani            #+#    #+#             */
/*   Updated: 2025/08/01 12:49:10 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	new_line_flag(char **args, int *no_newline)
{
	int	j;

	*no_newline = 0;
	j = 0;
	if (!args)
		return (0);
	while (args[j] && is_n_flag(args[j]))
	{
		*no_newline = 1;
		j++;
	}
	return (j);
}

int	builtin_echo(char **args)
{
	int	no_newline;
	int	j;

	no_newline = 0;
	if (!args)
		return (write(1, "\n", 1), 0);
	if (args[0])
	{
		j = new_line_flag(args, &no_newline);
		while (args[j])
		{
			if (no_newline == 1 && ft_strcmp(args[j], "\n") == 0)
			{
				j++;
				continue ;
			}
			write(1, args[j], ft_strlen(args[j]));
			if (args[j + 1] != NULL)
				write(1, " ", 1);
			j++;
		}
	}
	if (no_newline == 0)
		write(1, "\n", 1);
	return (0);
}
