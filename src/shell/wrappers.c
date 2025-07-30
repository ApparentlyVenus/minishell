/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:33:55 by odana             #+#    #+#             */
/*   Updated: 2025/07/30 13:48:28 by yitani           ###   ########.fr       */
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
		handle_error(shell, "tokenization failed", EXIT_FAILURE);
		return (0);
	}
	if (!validate_tokens(shell))
		return (0);
	return (1);
}

int	shell_parse(t_shell *shell)
{
	t_token	*token_copy;

	if (!shell || !shell->tokens)
		return (0);
	shell->current_phase = PHASE_PARSE;
	token_copy = shell->tokens;
	shell->ast = parse_input(&token_copy, shell->env);
	if (!shell->ast)
		return (0);
	return (1);
}

int	shell_execute(t_shell *shell)
{
	if (!shell || !shell->ast)
		return (0);
	shell->current_phase = PHASE_EXECUTE;
	execute_pipeline(shell);
	return (1);
}
