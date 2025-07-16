/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:47:00 by odana             #+#    #+#             */
/*   Updated: 2025/07/16 17:41:18 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_redir(t_token *token)
{
	if (token->type == TOKEN_HERDOC || token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_OUT_APPEND)
		return (1);
	return (0);
}

void	append_redir(t_redir **list, t_redir *new_redir)
{
	if (!new_redir)
		return ;
	new_redir->next = *list;
	*list = new_redir;
}

// /*
// ** add_arg_list
// ** Purpose: Adds a new t_arg to the list,
//			copying value and quoting info from t_token.
// ** Used variables: list (t_arg **), token (t_token *)
// ** Return: 1 on success, 0 on failure
// */
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

/*
** process_args
** Purpose: Converts a linked list of t_arg to a NULL-terminated array of t_arg*.
** Used variables: arg_list (t_arg *), count (int)
** Return: t_arg** (NULL-terminated array)
*/
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
