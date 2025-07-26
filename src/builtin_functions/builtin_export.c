/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:06:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/26 19:20:44 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_sorted_env(t_env **envp)
{
	t_env	*current;

	envp = do_something(envp);
	current = *envp;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

int	builtin_export(char **args, t_env **env)
{
	char	*equal_sign;
	int		failure[1];
	int		i;

	i = 0;
	failure[0] = 0;
	if (!args || !args[0])
		return (print_sorted_env(env), 0);
	if (args[0][0] == '=')
	{
		ft_putendl_fd("Not a valid identifier", 2);
		failure[0] = 1;
		i++;
	}
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
			export_helper(env, equal_sign, args[i], failure);
		else if (is_valid_key(args[i]))
			set_env_value(env, args[i], NULL);
		else
			failure[0] = 1;
		i++;
	}
	return (failure[0]);
}
