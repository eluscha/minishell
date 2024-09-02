/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:01:28 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/24 12:01:30 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_putnbr(char *dest, long n, int i)
{
	if (n == 0)
		ft_strlcpy(dest, "0", i + 1);
	else
	{
		if (n < 0)
		{
			dest[0] = '-';
			n = n *(-1);
		}
		dest[i] = '\0';
		i--;
		while (n > 0)
		{
			dest[i] = (n % 10) + 48;
			n = n / 10;
			i--;
		}
	}
	return (dest);
}

char	*ft_itoa(int n)
{
	long	i;
	int		size;
	char	*str;
	char	*res;

	size = 2;
	i = n;
	if (n < 0)
	{
		size = 3;
		i = i * (-1);
	}
	while (i > 9)
	{
		size ++;
		i = i / 10;
	}
	str = (char *) malloc(size);
	if (str == NULL)
		return (NULL);
	res = ft_putnbr(str, n, size - 1);
	return (res);
}
