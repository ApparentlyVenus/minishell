/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 01:33:35 by yitani            #+#    #+#             */
/*   Updated: 2025/06/28 22:40:55 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HANDLING_H
# define INPUT_HANDLING_H

int		is_operator(char c); // checking if its an operator
int		is_word_char(char c); // checking if its a word
int		get_input(char *stash); // getting the first prompt
char	*extract_word(char *input, int *pos); // extracting the word
t_token	*clean_word_token(char *word); // cleaning up the word from remaining quotes
int		is_closed(char *input , int pos); // omar 🧘🏼‍♂️ hay la na3rif if single quotes are closed
int		is_quotes(char c); // omarrr eza its even a quote in the first place
t_token	*tokenize_input(char *input); // tokenizing the input
t_token	*extract_operator_token(char *input, int *pos); // tokenizing the operators

#endif