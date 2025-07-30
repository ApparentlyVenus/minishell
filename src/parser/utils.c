/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:47:00 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 02:40:47 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_redir(t_token *token)
{
	if (token->type == TOKEN_HEREDOC || token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_OUT_APPEND)
		return (1);
	return (0);
}

void	append_redir(t_redir **list, t_redir *new_redir)
{
	t_redir	*current;

	if (!new_redir)
		return ;
	if (!*list)
	{
		*list = new_redir;
		new_redir->next = NULL;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_redir;
	new_redir->next = NULL;
}

int	add_arg_list(t_arg **list, t_token *token)
{
	t_arg	*cur;
	t_arg	*new;

	new = malloc(sizeof(t_arg));
	if (!new)
		return (0);
	new->value = ft_strdup(token->value);
	if (!new->value)
		return (free(new), 0);
	new->single_quotes = token->single_quotes;
	new->double_quotes = token->double_quotes;
	new->concat = token->concat;
	new->next = NULL;
	if (!*list)
		*list = new;
	else
	{
		cur = *list;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	return (1);
}

t_arg	**process_args(t_arg *arg_list, int count)
{
	t_arg	**args;
	t_arg	*current;
	int		i;

	args = (t_arg **)malloc(sizeof(t_arg *) * (count + 1));
	if (!args)
		return (free_arg(arg_list), NULL);
	i = 0;
	current = arg_list;
	while (current)
	{
		args[i++] = current;
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

int	get_token_priority(t_token *tok)
{
	if (!tok)
		return (100);
	else if (tok->type == TOKEN_AND || tok->type == TOKEN_OR)
		return (1);
	else if (tok->type == TOKEN_PIPE)
		return (2);
	else
		return (0);
}
