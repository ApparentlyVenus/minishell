/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:33:55 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 09:37:47 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	shell_tokenize(t_shell *shell, char *input)
{
	if (!shell || !input)
		return (0);
	shell->current_phase = PHASE_TOKEN;
	if (!tokenize_input(input, shell))
	{
		set_error(shell, "tokenization failed");
		return (0);
	}
	if (!validate_tokens(shell))
		return (0);
	return (1);
}

// In shell_parse function:
int	shell_parse(t_shell *shell)
{
	t_token	*token_copy;

	if (!shell || !shell->tokens)
		return (0);
	shell->current_phase = PHASE_PARSE;
	token_copy = shell->tokens;
	shell->ast = parse_input(&token_copy);
	if (!shell->ast)
	{
		set_error(shell, "parsing failed");
		return (0);
	}
	return (1);
}

// In shell_execute function:
int	shell_execute(t_shell *shell)
{
	if (!shell || !shell->ast)
		return (0);
	shell->current_phase = PHASE_EXECUTE;
	execute_pipeline(shell);
	return (1);
}
