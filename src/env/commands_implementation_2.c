/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_implementation_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:27:25 by yitani            #+#    #+#             */
/*   Updated: 2025/07/03 07:28:33 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_export(t_env **envp, t_token *args)
{
	t_env	*new_node;
	char	*equal_sign;
	int		i;

	i = 0;
	new_node = NULL;
	equal_sign = ft_strchr(args->value, '=');
	if (equal_sign)
	{
		new_node->equal = 1;
		new_node->value = ft_strdup(equal_sign + 1);
		new_node->key = ft_substr(args->value, 0, equal_sign - args->value);
		while (new_node->value)
		{
			if (new_node->value[i] == '$')
				new_node->value = expand_token_value_final(new_node->value, *envp);
			i++;
		}
		set_env_value(envp, new_node->key, new_node->value);
	}
}
