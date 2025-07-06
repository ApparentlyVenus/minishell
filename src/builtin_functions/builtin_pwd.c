/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:08:03 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 04:16:21 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_pwd(t_shell *shell)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		shell->exit_code = 1;
		return ;
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	shell->exit_code = 0;
	return ;
}
