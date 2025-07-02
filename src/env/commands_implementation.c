/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_implementation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:04 by yitani            #+#    #+#             */
/*   Updated: 2025/07/03 00:20:32 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	get_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (0);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (1);
}
