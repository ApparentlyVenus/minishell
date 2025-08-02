/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:12:36 by yitani            #+#    #+#             */
/*   Updated: 2025/08/02 23:04:17 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_overflow(const char *str)
{
	int			i;
	int			sign;
	long long	result;
	long long	prev;

	i = 0;
	sign = 1;
	result = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i - 1] == '-')
		sign = -1;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		prev = result;
		result = result * 10 + (str[i] - '0');
		if ((result / 10 != prev)
			|| (sign == 1 && result < 0)
			|| (sign == -1 && result > 0 && (-result) == 0))
			return (0);
		i++;
	}
	return (1);
}

int	size_of_arr(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	builtin_exit(char **args, t_shell *shell)
{
	long long	code;

	printf("exit\n");
	if (size_of_arr(args) >= 2)
	{
		if (size_of_arr(args) > 2)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			shell->exit_code = EXIT_FAILURE;
			return (shell->exit_code);
		}
		if (!ft_is_numeric(args[1]) || !check_overflow(args[1]))
		{
			ft_putendl_fd("minishell: exit: numeric argument required", 2);
			shell->exit_code = EXIT_MISUSE;
			shell_exit(shell, shell->exit_code);
		}
		code = ft_atoll(args[1]);
		shell->exit_code = (unsigned int)code;
	}
	shell_exit(shell, shell->exit_code);
	return (shell->exit_code);
}
