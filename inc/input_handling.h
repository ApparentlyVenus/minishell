/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 01:33:35 by yitani            #+#    #+#             */
/*   Updated: 2025/06/28 06:36:46 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HANDLING_H
# define INPUT_HANDLING_H

int		get_input(char *stash);
char	*extract_word (char *input, int *pos);
char	*clean_spaces(char *word);
t_token	*tokenize_input(char *input);

#endif