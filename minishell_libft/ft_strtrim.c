/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:27:36 by yitani            #+#    #+#             */
/*   Updated: 2025/05/21 16:31:02 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

static int	ft_is_set(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new;
	int		i;
	int		j;
	int		k;

	i = 0;
	k = 0;
	if (!s1)
	{
		new = malloc(1);
		return (new);
	}
	j = ft_strlen(s1) - 1;
	while (s1[i] && ft_is_set(set, s1[i]))
		i++;
	while (j >= i && ft_is_set(set, s1[j]))
		j--;
	new = (char *)malloc((j - i + 1) + 1);
	if (!new)
		return (NULL);
	while (i <= j)
		new[k++] = s1[i++];
	new[k] = '\0';
	return (new);
}
