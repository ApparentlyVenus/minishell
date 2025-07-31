/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:10:43 by yitani            #+#    #+#             */
/*   Updated: 2025/08/01 02:29:34 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	get_redir_info(t_token **tokens, int *type,
		char **filename, t_token **file_token)
{
	if (!*tokens)
		return (0);
	if ((*tokens)->type == TOKEN_REDIR_IN)
		*type = REDIR_IN;
	else if ((*tokens)->type == TOKEN_REDIR_OUT)
		*type = REDIR_OUT;
	else if ((*tokens)->type == TOKEN_REDIR_OUT_APPEND)
		*type = REDIR_OUT_APPEND;
	else if ((*tokens)->type == TOKEN_HEREDOC)
		*type = HERE_DOC;
	else
		return (0);
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (0);
	*file_token = *tokens;
	*filename = (*tokens)->value;
	*tokens = (*tokens)->next;
	return (1);
}

t_redir	*parse_redir(t_token **tokens, t_env *env)
{
	int		type;
	char	*filename;
	t_token	*file_token;
	t_redir	*redir;

	if (!get_redir_info(tokens, &type, &filename, &file_token))
		return (NULL);
	if (type == HERE_DOC)
		return (process_heredoc(filename, env,
				file_token->single_quotes, file_token->double_quotes));
	redir = create_redir_node(type, ft_strdup(filename));
	if (!redir)
		return (NULL);
	redir->single_quotes = file_token->single_quotes;
	redir->double_quotes = file_token->double_quotes;
	
	return (redir);
}
