/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:29:12 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:41:42 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	match(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (little[i] == big[j] && j < len)
	{
		i++;
		j++;
		if (little[i] == '\0')
			return (1);
	}
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	j;

	j = 0;
	if (*little == '\0')
		return ((char *)big);
	while ((big[j] != '\0') & (j < len))
	{
		if (*little == big[j] && match (&big[j], little, len - j) == 1)
			return ((char *)big + j);
		else
			j++;
	}
	return (0);
}
