/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:29:17 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:28:11 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*buf;
	unsigned char	c2;

	buf = s;
	c2 = c;
	while (n > 0)
	{
		*buf = c2;
		buf ++;
		n --;
	}
	return (s);
}
