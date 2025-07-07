/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 03:46:17 by yitani            #+#    #+#             */
/*   Updated: 2025/07/08 01:08:33 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	new_line_flag(t_token *args, int *i)
{
	int	j;
	
	*i = 0;
	j = 0;
	while (args && ft_strcmp(args->value, "-n") == 0)
	{
		args = args->next;
		*i = 1;
		j++;
	}
	return (j);
}

static int	builtin_echo_stdrd(char **args, t_exec *shell)
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
			write(1, args[j], strlen(args[j]));
			if (args[j + 1] != NULL)
				write(1, " ", 1);
		}
		j++;
	}
	if (i == 0)
		write(1, "\n", 1);
	return (0);
}

int	builtin_echo(char **args, t_exec *shell)
{
	shell->exit_code = builtin_echo_stdrd(args, shell);
	return (shell->exit_code);
}
