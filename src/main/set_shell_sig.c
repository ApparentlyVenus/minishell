/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_shell_sig.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:07:36 by yitani            #+#    #+#             */
/*   Updated: 2025/07/30 01:07:57 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_shell	**get_shell_ref(void)
{
	static t_shell *shell = NULL;
	return (&shell);
}

void	set_shell(t_shell *sh)
{
	*get_shell_ref() = sh;
}

t_shell	*get_shell(void)
{
	return (*get_shell_ref());
}
