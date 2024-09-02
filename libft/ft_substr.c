/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:51:59 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:12:11 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	size;
	char	*src;

	size = 0;
	if (s == 0)
		return (0);
	if (start >= ft_strlen(s))
		start = 0;
	else
		size = ft_strlen(s) - start;
	src = (char *)s + start;
	if (size > len)
		size = len;
	res = (char *)malloc(size + 1);
	if (res == 0)
		return (0);
	ft_strlcpy(res, src, size + 1);
	return (res);
}
