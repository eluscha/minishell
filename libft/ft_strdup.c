/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:02:35 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:26:56 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	size;
	char	*res;
	size_t	i;

	size = 0;
	i = 0;
	while (s[size] != '\0')
		size ++;
	res = (char *)malloc(size + 1);
	if (res == 0)
		return (0);
	while (i < size)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
