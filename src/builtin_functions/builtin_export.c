/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:06:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/30 01:27:41 by yitani           ###   ########.fr       */
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

static void	process_export_arg(char *arg, t_env **env, int *failure)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if ((equal_sign && ft_strncmp(arg, "_",
				equal_sign - arg) == 0 && equal_sign - arg == 1)
		|| (!equal_sign && ft_strcmp(arg, "_") == 0))
	{
		ft_putendl_fd("minishell: export: `_': not a valid identifier",
			STDERR_FILENO);
		failure[0] = 1;
	}
	else if (equal_sign)
		export_helper(env, equal_sign, arg, failure);
	else if (is_valid_key(arg))
		set_env_value(env, arg, NULL);
	else
		failure[0] = 1;
}

int	builtin_export(char **args, t_env **env)
{
	int		failure[1];
	int		i;

	failure[0] = 0;
	if (!args || !args[0])
		return (print_sorted_env(env), 0);
	if (args[0][0] == '=')
	{
		ft_putendl_fd("minishell: export: not a valid identifier",
			STDERR_FILENO);
		failure[0] = 1;
		i = 1;
	}
	else
		i = 0;
	while (args[i])
		process_export_arg(args[i++], env, failure);
	return (failure[0]);
}
