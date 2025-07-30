/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:59:33 by odana             #+#    #+#             */
/*   Updated: 2025/07/31 02:47:29 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

// main tokenization function

t_token	**tokenize_input(char *input, t_shell *shell);

// helpers

t_token	*clean_word_token(char *word);
t_token	*extract_operator_token(char *input, int *pos);
t_token	*handle_word_token(char *input, int *i, t_shell *shell);
t_token	*create_next_token(char *input, int *i, t_shell *shell);
t_token	*handle_empty_quotes(char *input, int *i);
char	*trim_quotes(char *word, t_token *token);
void	toggle_quotes(char c, int *in_single, int *in_double);
int		continue_word(char c, int in_single, int in_double);

// quotes and complex tokens

int		has_adjacent_quotes(char *input, int pos);
char	*extract_quote_segment(char *input, int *pos,
			int *s_quote, int *d_quote);
t_token	*create_segment_token(char *segment, int s_quote, int d_quote);
t_token	*tokenize_complex_word(char *input, int *i, t_shell *shell);
void	add_tokens(t_token *new_token, t_shell *shell, int had_space);

// bool helpers

int		is_operator(char c);
int		is_word_char(char c);
int		has_wildcard(char *word);
int		is_closed(char *input, int pos);
int		is_quotes(char c);

// utils 

void	free_tokens(t_token **head);
char	*extract_word(char *input, int *pos);
void	skip_spaces(char *input, int *pos);

// validation functions

// main validation function for all validations

int		validate_tokens(t_shell *shell);

// validation helpers

int		redirection_validation(t_token **token);
int		pipe_validation(t_token **token);
int		valid_heredoc(t_token **token);
int		valid_logic_op(t_token **token);
int		valid_parentheses(t_token **tokens);
int     valid_quotes(t_token **tokens);

// validation utils

int		is_redirection(t_token_type type);
int		is_logic_op(t_token_type type);
int		wildcard_count(char *word);
void	token_add_back(t_token **lst, t_token *new);

#endif