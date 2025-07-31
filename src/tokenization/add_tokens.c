/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:44:28 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 17:47:19 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	process_token_list(t_token *new_token, t_shell *shell,
		int had_space)
{
	t_token	*token_list;
	t_token	*current;
	int		is_first_in_group;

	token_list = new_token;
	is_first_in_group = 1;
	while (token_list)
	{
		current = token_list;
		token_list = token_list->next;
		current->next = NULL;
		if (is_first_in_group)
		{
			current->concat = !had_space && shell->tokens;
			is_first_in_group = 0;
		}
		else
			current->concat = 1;
		token_add_back(&shell->tokens, current);
	}
}

void	add_tokens(t_token *new_token, t_shell *shell, int had_space)
{
	if (new_token->next)
		process_token_list(new_token, shell, had_space);
	else
	{
		new_token->concat = !had_space && shell->tokens;
		token_add_back(&shell->tokens, new_token);
	}
}
