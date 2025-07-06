/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:06:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 04:45:33 by yitani           ###   ########.fr       */
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

void	builtin_export(t_shell *shell, t_token *args)
{
	char	*equal_sign;
	int		failed;

	failed = 0;
	if (!args || !args->next)
	{
		print_sorted_env(shell->env);
		shell->exit_code = 0;
		return ;
	}
	while (args)
	{
		equal_sign = ft_strchr(args->value, '=');
		if (equal_sign)
			export_helper(shell->env, equal_sign, args);
		else if (is_valid_key(args->value))
			set_env_value(shell->env, args->value, NULL);
		else
			failed = 1;
		args = args->next;
	}
	shell->exit_code = failed;
}
