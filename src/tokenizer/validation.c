/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:05:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/02 17:49:37 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	redirection_validation(t_token **token)
{
	t_token	*current;
	t_token	*prev;

	prev = NULL;
	current = (*token);
	
	while (current)
	{
		if (is_redirection(current->type)
			&& current->next != NULL && is_redirection(current->next->type))
			return (0);
		if (is_redirection(current->type)
			&& (prev == NULL || prev->type != TOKEN_WORD))
			return (0);
		prev = current;
		current = current->next;
	}
	return (1);
}

int	pipe_validation(t_token **token)
{
	t_token	*current;
	t_token	*prev;

	current = (*token);
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_PIPE
			&& (prev == NULL || current->next == NULL
				|| current->next->type == TOKEN_PIPE))
			return (0);
		prev = current;
		current = current->next;
	}
	return (1);
}

int	valid_heredoc(t_token **token)
{
	t_token	*current;
	t_token	*prev;

	current = (*token);
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_HERDOC
			&& (prev == NULL || current->next == NULL
				|| current->next->type != TOKEN_WORD))
			return (0);
		prev = current;
		current = current->next;
	}
	return (1);
}

int	valid_logic_op(t_token **token)
{
	t_token	*current;
	t_token	*prev;

	current = (*token);
	prev = NULL;
	while (current)
	{
		if (is_logic_op(current->type)
			&& (prev == NULL || current->next == NULL
				|| is_logic_op(current->next->type)))
			return (0);
		prev = current;
		current = current->next;
	}
	return (1);
}

int	valid_wildcard(t_token **token)
{
	t_token	*current;

	current = (*token);
	while (current)
	{
		if (current->type == TOKEN_WORD && has_wildcard(current->value) > 1)
		{
			return (0);
		}
		current = current->next;
	}
	return (1);
}