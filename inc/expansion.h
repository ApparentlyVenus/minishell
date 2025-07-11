/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 09:32:24 by odana             #+#    #+#             */
/*   Updated: 2025/07/11 16:23:52 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

# define ERROR -1
# define NO_CHANGE 0
# define SPLIT_SUCCESS 1
# define TOKEN_REMOVED 2


// wildcard matching algorithm

int	    count_star_matches(const char *pattern);
int	    fill_star_matches(const char *pattern, char **matches,
                    int expect_count);
int	    collect_star_matches(const char *pattern, char ***matches);
int	    handle_star_case(const char **p, const char *f,
    				const char **star_p, const char **star_f);
int	    try_backtrack(const char **p, const char **f,
				const char *star_p, const char **star_f);
int	    match_star_pattern(const char *pattern, const char *filename);

// dollar expansion

char	*expand_tilde(char *value, t_env *env);
char	*extract_variable(char *value, t_env *env, int *i);
char	*handle_dollar_expansion(char *result, char *value, t_env *env, int *i);
char	*expand_variables(char *value, t_env *env);
t_token *process_word_token(t_token **tokens, t_token *current,
								   t_token *prev, t_env *env);
t_token	**expand_tokens(t_token **tokens, t_env *env);

// utils 

int	is_valid_var_start(char c);
int	is_valid_var_char(char c);
char	*append_string(char *dest, const char *to_append);
int has_wildcard(char *str);
int contains_whitespace(char *str);

// wildcard expansion

void sort_star_matches(char **matches, int count);
t_token *create_star_tokens(t_token *current, char **matches, int count);
t_token	*init_star_token(char *match);
t_token *expand_wildcard(t_token *current);

// word splitting

int handle_empty_token(t_token **tokens, t_token *current, t_token *prev);
t_token *init_new_token(char *word, t_token *last_inserted);
t_token *insert_split_words(t_token *current, char **split_words);
int handle_word_splitting(t_token **tokens, t_token *current, t_token *prev, char **split_words);
int apply_word_splitting(t_token **tokens, t_token *current, t_token *prev);

#endif