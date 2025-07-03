/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:55:58 by yitani            #+#    #+#             */
/*   Updated: 2025/07/03 05:04:23 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*s01;
	const unsigned char	*s02;

	s01 = (const unsigned char *)s1;
	s02 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (s01[i] != s02[i])
		{
			return (s01[i] - s02[i]);
		}
		i++;
	}
	return (0);
}
