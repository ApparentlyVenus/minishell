/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:03 by yitani            #+#    #+#             */
/*   Updated: 2025/07/28 21:28:37 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_pwd(t_env **env)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		path = get_env_value(*env, "PWD");
		if (!path)
			return (ft_putendl_fd("pwd: error retrieving current directory", 2),
				1);
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (0);
}
