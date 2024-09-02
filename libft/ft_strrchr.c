/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:10:16 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:35:16 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		mybool;
	char	*pntr;
	char	a;

	a = (char)c;
	mybool = 0;
	while (*s != '\0')
	{
		if (*s == a)
		{
			mybool = 1;
			pntr = (char *)s;
		}
		s++;
	}
	if (mybool == 1)
		return (pntr);
	if (a == '\0' && *s == '\0')
		return ((char *)s);
	else
		return (0);
}
