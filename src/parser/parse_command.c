/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:34:33 by yitani            #+#    #+#             */
/*   Updated: 2025/07/31 01:38:00 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	process_word_token(t_token **tokens, t_arg **arg_list, int *count)
{
	if (!add_arg_list(arg_list, *tokens))
		return (0);
	(*count)++;
	*tokens = (*tokens)->next;
	return (1);
}

static int	process_redir_token(t_token **tokens, t_env *env,
	t_redir **redir_list)
{
	t_redir	*redir;

	redir = parse_redir(tokens, env);
	if (!redir)
		return (0);
	append_redir(redir_list, redir);
	return (1);
}

static int	parse_tokens_loop(t_token **tokens, t_env *env,
	t_arg **arg_list, t_parse_data *data)
{
	while (*tokens && ((*tokens)->type == TOKEN_WORD || is_redir(*tokens)))
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (!process_word_token(tokens, arg_list, &data->count))
				return (0);
		}
		else if (is_redir(*tokens))
		{
			if (!process_redir_token(tokens, env, &data->redir_list))
				return (0);
		}
	}
	return (1);
}

t_node	*parse_command(t_token **tokens, t_env *env)
{
	t_parse_data	data;
	t_arg			*arg_list;

	data.count = 0;
	arg_list = NULL;
	data.redir_list = NULL;
	if (!parse_tokens_loop(tokens, env, &arg_list, &data))
		return (free_arg(arg_list), free_redir(data.redir_list), NULL);
	if (data.count == 0 && !data.redir_list)
		return (NULL);
	return (create_cmd_node(process_args(arg_list, data.count),
			data.redir_list));
}
