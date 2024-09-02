/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:35:29 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/23 16:49:07 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	join(char *res, char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[i] = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	max;
	char	*res;

	max = -1;
	if (((max - ft_strlen(s1)) - 1) < ft_strlen(s2))
		return (0);
	res = (char *) malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (res == 0)
		return (0);
	join (res, s1, s2);
	return (res);
}
