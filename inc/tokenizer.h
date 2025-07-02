/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:59:33 by odana             #+#    #+#             */
/*   Updated: 2025/07/03 00:46:07 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

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
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				single_quotes;
	int				double_quotes;
	struct s_token	*next;
}	t_token;

// functions

int		is_operator(char c); // checking if its an operator
void	skip_spaces(char *input, int *pos); // slipping spaces
int		is_word_char(char c); // checking if its a word
int		get_input(char *stash); // getting the first prompt
char	*extract_word(char *input, int *pos); // extracting the word
t_token	*clean_word_token(char *word);
int		is_closed(char *input, int pos);
int		is_quotes(char c); // omarrr eza its even a quote in the first place
t_token	**tokenize_input(char *input, t_token **token); // tokenizing the input
t_token	*extract_operator_token(char *input, int *pos);
void	free_tokens(t_token **head); // free **struct
t_token	*handle_any_word(char *input, char *word, int *i, t_token *new_token);

int		has_wildcard(char *word);
int		is_logic_op(t_token_type type);
int		has_wildcard(char *word);

int		redirection_validation(t_token **token);
int		pipe_validation(t_token **token);
int		valid_heredoc(t_token **token);
int		valid_logic_op(t_token **token);
int		valid_wildcard(t_token **token);

#endif