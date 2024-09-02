/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:32:21 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/22 16:25:48 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			max;
	unsigned char	*res;
	size_t			total;

	max = (size_t)-1;
	if (nmemb == 0 || size == 0)
		total = 1;
	else if ((max / size) < nmemb)
		return (0);
	else
		total = nmemb * size;
	res = malloc(total);
	if (res == 0)
		return (0);
	while (total > 0)
	{
		res[total - 1] = 0;
		total --;
	}
	return (res);
}
