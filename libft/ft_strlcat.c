/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:58:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:50:02 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_lng;
	size_t	src_lng;

	dst_lng = 0;
	src_lng = 0;
	while (dst[dst_lng] != '\0' && dst_lng < size)
		dst_lng ++;
	while (src[src_lng] != '\0')
		src_lng++;
	if (size == 0 || (dst_lng >= (size - 1)))
		return (dst_lng + src_lng);
	while (*dst != '\0' && size > 1)
	{
		dst++;
		size--;
	}
	while (*src != '\0' && size > 1)
	{
		*dst = *src;
		dst++;
		src++;
		size--;
	}
	*dst = '\0';
	return (dst_lng + src_lng);
}
