/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:05:19 by yitani            #+#    #+#             */
/*   Updated: 2025/07/30 13:39:07 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	validate_tokens(t_shell *shell)
{
	if (!redirection_validation(&shell->tokens))
		return (handle_error(shell,
				"minishell: unexpected token near `redirection`", EXIT_MISUSE), 0);
	if (!pipe_validation(&shell->tokens))
		return (handle_error(shell, "minishell: unexpected token near `|`", EXIT_MISUSE), 0);
	if (!valid_heredoc(&shell->tokens))
		return (handle_error(shell,
				"minishell: unexpected token near `heredoc`", EXIT_MISUSE), 0);
	if (!valid_logic_op(&shell->tokens))
		return (handle_error(shell,
				"minishell: unexpected token near `logical operator`", EXIT_MISUSE), 0);
	if (!valid_parentheses(&shell->tokens))
		return (handle_error(shell,
				"minishell: unmatched parentheses", EXIT_MISUSE), 0);
	return (1);
}

int	redirection_validation(t_token **token)
{
	t_token	*current;

	current = (*token);
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (current->next == NULL || current->next->type != TOKEN_WORD)
				return (0);
			if (is_redirection(current->next->type))
				return (0);
		}
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

	current = (*token);
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (current->next == NULL || current->next->type != TOKEN_WORD)
				return (0);
		}
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
