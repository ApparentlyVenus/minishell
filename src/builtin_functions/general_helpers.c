/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 03:32:22 by yitani            #+#    #+#             */
/*   Updated: 2025/07/28 21:30:06 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	is_valid_key(const char *key)
{
	int	i;

	if ((!key || !ft_isalpha(key[0]) || key[0] == '_'))
		return (ft_putendl_fd("minishell: export: not a valid identifier", 2),
			0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (ft_putendl_fd("minishell: export: not a valid identifier",
					2), 0);
		i++;
	}
	if (key[i] == '-')
		return (ft_putendl_fd("minishell: export: not a valid identifier",
				2), 0);
	return (1);
}
