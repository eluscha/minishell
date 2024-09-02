/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:33:23 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:15:29 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*src;
	unsigned char	a;

	a = (unsigned char) c;
	src = (unsigned char *) s;
	while (n > 0)
	{
		if (*src == a)
			return (src);
		src++;
		n--;
	}
	return (0);
}
