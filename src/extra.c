/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 08:59:48 by yitani            #+#    #+#             */
/*   Updated: 2025/07/02 23:59:01 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// im displaying "minishell$" at the start of every prompt;
// if the user enters CTRL + D this will put buffer to NULL
// thats why we should clean up and exit;
// clean up and exit doest have anything specified yet,
// it should contain the struct and all memory allocations;

#include "../inc/minishell.h"

int	get_input(char *stash)
{
	char	*buffer;

	buffer = NULL;
	buffer = readline("minishell$ ");
	if (!buffer)
	{
		return (clean_up_and_exit());
	}
	if (ft_strlen(buffer) > 0)
	{
		add_history(buffer);
		ft_strlcpy(stash, buffer, ft_strlen(buffer));
		free(buffer);
		return (1);
	}
	else
		return (0);
}
