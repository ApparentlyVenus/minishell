/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:23:46 by yitani            #+#    #+#             */
/*   Updated: 2025/07/27 21:27:55 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_fail_new_pwd(char *new_pwd, char *pwd_env)
{
	if (!new_pwd)
	{
		if (!pwd_env)
			new_pwd = ft_strdup("/");
		else
			new_pwd = ft_strjoin(pwd_env, "/..");
	}
}

void	handle_fail_old_pwd(char *old_pwd, char *pwd_env)
{
	if (!old_pwd)
	{
		if (!pwd_env)
			old_pwd = ft_strdup("");
		else
			old_pwd = ft_strdup(pwd_env);
	}
}