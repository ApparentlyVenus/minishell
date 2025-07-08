/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 04:00:07 by yitani            #+#    #+#             */
/*   Updated: 2025/07/08 11:14:57 by yitani           ###   ########.fr       */
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

int	wildcard_count(char *word)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (word[i])
	{
		if (word[i] == '*')
		{
			count++;
		}
		i++;
	}
	return (count);
}

void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
	{
		last = last->next;
	}
	last->next = new;
}
