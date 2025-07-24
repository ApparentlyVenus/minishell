/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 03:46:17 by yitani            #+#    #+#             */
/*   Updated: 2025/07/25 02:12:03 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	new_line_flag(char **args, int *no_newline)
{
	int	j;

	*no_newline = 0;
	j = 0;
	if (!args)
		return (0);
	while (args[j] && ft_strcmp(args[j], "-n") == 0)
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
