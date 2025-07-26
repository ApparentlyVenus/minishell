/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:51:57 by odana             #+#    #+#             */
/*   Updated: 2025/07/26 12:01:13 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*remove_all_quotes(char *str)
{
	char	*result;

	if (!str)
		return (NULL);
	result = remove_quotes(str);
	if (result)
	{
		if (ft_strcmp(result, str) != 0
			|| (!ft_strchr(str, '"') && !ft_strchr(str, '\'')))
			return (result);
		free(result);
	}
	return (remove_quotes_compex(str));
}

char	*remove_quotes_extra(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (ft_strdup(str));
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (process_quotes(str[i]))
			i++;
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

int	process_quote(char c)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	if (c == '\'' && !in_double)
	{
		in_single = !in_single;
		return (1);
	}
	else if (c == '"' && !in_single)
	{
		in_double = !in_double;
		return (1);
	}
	return (0);
}

char	*remove_quotes(char *filename)
{
	char	*result;
	int		len;

	result = NULL;
	len = ft_strlen(filename);
	if (!filename)
		return (NULL);
	if (len >= 2 && filename[0] == '\'' && filename[len - 1] == '\'')
	{
		result = ft_substr(filename, 1, len -2);
		return (result);
	}
	if (len >= 2 && filename[0] == '"' && filename[len - 1] == '"')
	{
		result = ft_substr(filename, 1, len -2);
		return (result);
	}
	if (ft_strchr(filename, '"') || ft_strchr(filename, '\''))
	{
		result = remove_adjacent_quotes(filename);
		return (result);
	}
	return (result);
}

char	*remove_adjacent_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (ft_strdup(str));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			i++;
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
