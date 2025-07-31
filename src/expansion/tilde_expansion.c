/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 02:15:44 by yitani            #+#    #+#             */
/*   Updated: 2025/08/01 02:37:13 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_tilde(char *str, t_env *env)
{
	char	*home;
	char	*result;
	char	*suffix;

	if (!str || str[0] != '~')
		return (ft_strdup(str));
	if (str[1] == '\0' || str[1] == '/')
	{
		home = get_env_value(env, "HOME");
		if (!home)
			home = getenv("HOME");
		if (!home)
			return (ft_strdup(str));
		if (str[1] == '\0')
			return (ft_strdup(home));
		suffix = ft_strdup(str + 1);
		if (!suffix)
			return (ft_strdup(str));
		result = ft_strjoin(home, suffix);
		free(suffix);
		if (!result)
			return (ft_strdup(str));
		return (result);
	}
	return (ft_strdup(str));
}
