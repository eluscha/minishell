/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:02:04 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/23 18:02:55 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_char(char c, char const *set)
{
	while (*set != '\0')
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static char	*allocate(int i, int j, char const *s1)
{
	char	*res;

	if (j == i && s1[j] == '\0')
		res = (char *) malloc (1);
	else if (j == i)
		res = (char *) malloc(2);
	else
		res = (char *) malloc (j + 2 - i);
	return (res);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		j;
	int		i;
	char	*res;

	i = 0;
	j = ft_strlen(s1);
	while (check_char(s1[i], set) == 1 && s1[i] != '\0')
		i++;
	if (j > i)
		j --;
	while (j > i && check_char(s1[j], set) == 1)
		j--;
	res = allocate(i, j, s1);
	if (res == 0)
		return (0);
	ft_strlcpy (res, &s1[i], j + 2 - i);
	return (res);
}
