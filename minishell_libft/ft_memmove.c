/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:44:02 by yitani            #+#    #+#             */
/*   Updated: 2025/05/23 16:39:19 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ds;
	unsigned char	*sc;
	size_t			i;

	ds = (unsigned char *)dest;
	sc = (unsigned char *)src;
	if (dest > src)
	{
		i = n;
		while (i > 0)
		{
			ds[i - 1] = sc[i - 1];
			i--;
		}
	}
	else
	{
		i = 0;
		while (i < n)
		{
			ds[i] = sc[i];
			i++;
		}
	}
	return (dest);
}
