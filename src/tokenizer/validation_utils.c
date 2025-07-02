/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 04:00:07 by yitani            #+#    #+#             */
/*   Updated: 2025/07/02 17:49:37 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_redirection(t_token_type type)
{
	if (type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT_APPEND)
		return (1);
	else
		return (0);
}

int	is_logic_op(t_token_type type)
{
	if (type == TOKEN_AND || type == TOKEN_OR)
		return (1);
	else
		return (0);
}

int	has_wildcard(char *word)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (word)
	{
		if (word[i] == '*')
		{
			count++;
		}
		i++;
	}
	return (count);
}