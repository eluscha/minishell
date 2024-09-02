/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:55:55 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:45:42 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	cpy(const unsigned char *src2, unsigned char *dest2, size_t i)
{
	while (i > 0)
	{
		i --;
		dest2[i] = src2[i];
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*src2;
	unsigned char		*dest2;
	size_t				i;

	if ((dest == 0 && src == 0) || n == 0)
		return (dest);
	i = 0;
	dest2 = dest;
	src2 = src;
	if ((dest - src) <= 0)
	{
		while (i < n)
		{
			dest2[i] = src2[i];
			i++;
		}
	}
	else
		cpy(src2, dest2, n);
	return (dest);
}
