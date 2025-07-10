/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 09:38:35 by odana             #+#    #+#             */
/*   Updated: 2025/07/10 15:27:55 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int has_wildcard(char *str)
{
	int i;

	if (!str)
		return (0);
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

/*
 * append_string_to_result - Appends string to result and frees old result
 * 
 * @dest: Destination string (will be freed)
 * @to_append: String to append
 * @return: New concatenated string
 */
char	*append_string(char *dest, const char *to_append)
{
	char	*new;

	new = ft_strjoin(dest, to_append);
	free(dest);
	return (new);
}