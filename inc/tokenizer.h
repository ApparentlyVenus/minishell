/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:59:33 by odana             #+#    #+#             */
/*   Updated: 2025/07/08 11:17:24 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_OUT_APPEND,
	TOKEN_HERDOC,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_WILDCARD,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				single_quotes;
	int				double_quotes;
	int				priority;
	int				has_wildcard;
	struct s_token	*next;
}	t_token;

// functions

int		is_operator(char c);
void	skip_spaces(char *input, int *pos);
int		is_word_char(char c);
int		get_input(char *stash);
char	*extract_word(char *input, int *pos);
t_token	*clean_word_token(char *word);
int		is_closed(char *input, int pos);
int		is_quotes(char c);
t_token	**tokenize_input(char *input, t_token **token);
t_token	*extract_operator_token(char *input, int *pos);
void	free_tokens(t_token **head);
t_token	*handle_any_word(char *input, char *word, int *i, t_token *new_token);

int		has_wildcard(char *word);
int		is_logic_op(t_token_type type);
int		is_redirection(t_token_type type);
int		wildcard_count(char *word);
void	token_add_back(t_token **lst, t_token *new);

int		redirection_validation(t_token **token);
int		pipe_validation(t_token **token);
int		valid_heredoc(t_token **token);
int		valid_logic_op(t_token **token);
int		valid_wildcard(t_token **token);

#endif