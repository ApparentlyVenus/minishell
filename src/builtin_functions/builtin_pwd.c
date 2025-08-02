/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:03 by yitani            #+#    #+#             */
/*   Updated: 2025/08/02 21:39:35 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_pwd(t_env **env, char **args)
{
	char	*path;

	if (count_args(args) > 1)
	{
		if (args[1][0] == '-' && args[1][1] != '\0')
			return(printf("\'%c\': invalid option\n", args[1][1]), 2);
	}
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
