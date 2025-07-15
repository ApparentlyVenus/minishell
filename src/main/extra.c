/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 08:59:48 by yitani            #+#    #+#             */
/*   Updated: 2025/07/15 18:54:43 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// im displaying "minishell$" at the start of every prompt;
// if the user enters CTRL + D this will put buffer to NULL
// thats why we should clean up and exit;
// clean up and exit doest have anything specified yet,
// it should contain the struct and all memory allocations;

#include "../inc/minishell.h"

char *get_input_line(void)
{
	char *buffer;

	buffer = readline("minishell@~$ ");
	if (!buffer)
		return (NULL);
	if (ft_strlen(buffer) > 0)
		add_history(buffer);
	return (buffer);
}

void update_shlvl(t_shell *shell)
{
	char	*current_shlvl;
	char	*new_shlvl_str;
	char	*export_arg;
	int		level;

	if (!shell || !shell->env)
		return ;
	current_shlvl = get_env_value(shell->env, "SHLVL");
	if (!current_shlvl)
		level = 1;
	else
		level = ft_atoi(current_shlvl) + 1;
	new_shlvl_str = ft_itoa(level);
	if (!new_shlvl_str)
		return ;
	export_arg = ft_strjoin("SHLVL=", new_shlvl_str);
	free(new_shlvl_str);
	if (!export_arg)
		return ;
	set_env_value(&shell->env, "SHLVL", ft_strchr(export_arg, '=') + 1);
	free(export_arg);
}
