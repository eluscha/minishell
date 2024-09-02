/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:48:03 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:31:07 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	a;

	a = (char)c;
	while (*s != '\0')
	{
		if (*s == a)
			return ((char *)s);
		s++;
	}
	if (*s == a)
		return ((char *)s);
	return (0);
}
