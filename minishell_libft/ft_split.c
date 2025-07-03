/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:42:13 by yitani            #+#    #+#             */
/*   Updated: 2025/07/03 05:04:23 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_len(const char *str, char sep)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != sep)
		len++;
	return (len);
}

static char	**handle_malloc_error(char **strs, int j)
{
	while (j > 0)
	{
		free(strs[j - 1]);
		j--;
	}
	free(strs);
	return (NULL);
}

static char	*splitter(char const *str, char charset)
{
	int		i;
	int		len;
	char	*dup;

	dup = 0;
	i = 0;
	if (!str)
		return (0);
	len = word_len(str, charset);
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static int	count_words(const char *str, char sep)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != sep)
		{
			count++;
			while (str[i] && str[i] != sep)
				i++;
		}
		else
			i++;
	}
	return (count);
}

char	**ft_split(char const *str, char charset)
{
	int		j;
	int		i;
	char	**strs;

	strs = (char **)malloc(sizeof(char *) * (count_words(str, charset) + 1));
	if (!strs || !str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != charset)
		{
			strs[j] = splitter(&str[i], charset);
			if (!strs[j++])
				return (handle_malloc_error(strs, j));
			while (str[i] && str[i] != charset)
				i++;
		}
		else
			i++;
	}
	strs[j] = NULL;
	return (strs);
}
