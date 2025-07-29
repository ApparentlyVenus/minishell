/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:23:46 by yitani            #+#    #+#             */
/*   Updated: 2025/07/28 21:28:46 by odana            ###   ########.fr       */
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
