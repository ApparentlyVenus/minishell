/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 03:46:17 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 09:26:44 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token	*new_line_flag(t_token *args, int *i)
{
	*i = 0;
	while (args && ft_strcmp(args->value, "-n") == 0)
	{
		args = args->next;
		*i = 1;
	}
	return (args);
}

static int	builtin_echo_stdrd(t_token *args, t_exec *shell)
{
	int	i;

	if (args->next)
	{
		args = args->next;
		args = new_line_flag(args, &i);
		while (args)
		{
			if (args->type == TOKEN_WORD)
			{
				if (i == 1 && ft_strcmp(args->value, "\n") == 0)
				{
					args = args->next;
					continue ;
				}
				write(1, args->value, strlen(args->value));
				if (args->next && args->next->type == TOKEN_WORD)
					write(1, " ", 1);
			}
			args = args->next;
		}
	}
	if (i == 0)
		write(1, "\n", 1);
	return (0);
}

int	builtin_echo(t_token *tokens, t_exec *shell)
{
	t_token	*expanded_tokens;

	expanded_tokens = echo_util(tokens, *shell->env);
	shell->exit_code = builtin_echo_stdrd(expanded_tokens, shell);
	return (shell->exit_code);
}
