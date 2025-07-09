/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:06:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/09 17:34:56 by yitani           ###   ########.fr       */
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

int	builtin_export(t_exec *shell, char **args)
{
	char	*equal_sign;
	int		failed;
	int		i;

	i = 0;
	failed = 0;
	if (!args || !args[0])
	{
		print_sorted_env(shell->env);
		shell->exit_code = 0;
		return ;
	}
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
			export_helper(shell->env, equal_sign, args[i]);
		else if (is_valid_key(args[i]))
			set_env_value(shell->env, args[i], NULL);
		else
			failed = 1;
		i++;
	}
	return (failed);
}
