/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:33:55 by odana             #+#    #+#             */
/*   Updated: 2025/07/16 17:38:56 by yitani           ###   ########.fr       */
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

int	shell_expand(t_shell *shell)
{
	if (!shell || !shell->tokens)
		return (0);
	shell->current_phase = PHASE_EXPAND;
	if (!expand_tokens(&shell->tokens, shell->env))
	{
		set_error(shell, "expansion failed");
		return (0);
	}
	return (1);
}

int	shell_parse(t_shell *shell)
{
	if (!shell || !shell->tokens)
		return (0);
	shell->current_phase = PHASE_PARSE;
	shell->ast = parse_input(&shell->tokens);
	if (!shell->ast)
	{
		set_error(shell, "parsing failed");
		return (0);
	}
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
