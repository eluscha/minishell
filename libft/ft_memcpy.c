/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:10:42 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:47:45 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest2;
	const unsigned char	*src2;

	if (dest == 0 && src == 0)
		return (dest);
	dest2 = dest;
	src2 = src;
	while (n > 0)
	{
		*dest2 = *src2;
		dest2 ++;
		src2 ++;
		n --;
	}
	return (dest);
}
