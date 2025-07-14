/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 08:59:48 by yitani            #+#    #+#             */
/*   Updated: 2025/07/13 18:02:01 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// im displaying "minishell$" at the start of every prompt;
// if the user enters CTRL + D this will put buffer to NULL
// thats why we should clean up and exit;
// clean up and exit doest have anything specified yet,
// it should contain the struct and all memory allocations;

#include "../inc/minishell.h"

int	get_input(char *stash)
{
	char	*buffer;

	buffer = NULL;
	buffer = readline("minishell@~$ ");
	if (!buffer)
	{
		return (clean_up_and_exit());
	}
	if (ft_strlen(buffer) > 0)
	{
		add_history(buffer);
		ft_strlcpy(stash, buffer, ft_strlen(buffer) + 1);
		free(buffer);
		return (1);
	}
	else
		return (0);
}

void	update_shlvl(t_exec *shell)
{
	char	*val[3];
	int		level;

	val[0] = "SHLVL=";
	val[1] = get_env_value(*(shell->env), "SHLVL");
	val[2] = NULL;
	level = ft_atoi(val[1]) + 1;
	if (!level)
		builtin_export(shell, "SHLVL=1");
	else
	{
		val[1] = ft_itoa(level);
		builtin_export(shell, val);
	}
}
