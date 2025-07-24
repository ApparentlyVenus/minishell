/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:31:58 by odana             #+#    #+#             */
/*   Updated: 2025/07/25 02:07:54 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cleanup_tokens(t_shell *shell)
{
	if (!shell || !shell->tokens)
		return ;
	free_tokens(&shell->tokens);
	shell->tokens = NULL;
}

void	cleanup_ast(t_shell *shell)
{
	if (!shell || !shell->ast)
		return ;
	free_node(shell->ast);
	shell->ast = NULL;
}

void	cleanup_exec(t_shell *shell)
{
	if (!shell || !shell->exec_ctx)
		return ;
	free_exec(shell->exec_ctx);
	shell->exec_ctx = NULL;
}

void	free_env(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = temp;
	}
}
