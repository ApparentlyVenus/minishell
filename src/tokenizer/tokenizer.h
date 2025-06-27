/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:59:33 by odana             #+#    #+#             */
/*   Updated: 2025/06/27 22:10:15 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZE_H

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_OUT_APPEND,
    TOKEN_HERDOC,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				single_quotes;
	int				double_quotes;
	struct s_token	*next;
}	t_token;

#endif