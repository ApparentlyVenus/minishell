/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 13:54:07 by odana             #+#    #+#             */
/*   Updated: 2025/07/27 15:19:59 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	current_directory_exists(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	free(cwd);
	return (1);
}

char	*safe_getcwd(t_env **env)
{
	char	*cwd;
	char	*home;

	cwd = getcwd(NULL, 0);
	if (cwd)
		return (cwd);
	ft_putendl_fd("minishell: current directory removed, returning to HOME", 2);
	home = get_env_value(*env, "HOME");
	if (!home)
	{
		if (chdir("/") == 0)
			return (set_env_value(env, "PWD", "/"), ft_strdup("/"));
		return (NULL);
	}
	if (chdir(home) == 0)
		return (set_env_value(env, "PWD", home), ft_strdup(home));
	if (chdir("/") == 0)
		return (set_env_value(env, "PWD", "/"), ft_strdup("/"));
	return (NULL);
}
