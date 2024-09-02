/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:21:56 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:32:13 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned const char	*is1;
	unsigned const char	*is2;

	is1 = (unsigned const char *)s1;
	is2 = (unsigned const char *)s2;
	if (n == 0)
		return (0);
	while (*is1 != '\0' && *is2 == *is1 && n > 1)
	{
		is1++;
		is2++;
		n--;
	}
	return (*is1 - *is2);
}
