/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 13:59:33 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/24 13:59:39 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	size;
	unsigned int	i;
	char			*arr;

	if (s == NULL)
		return (0);
	size = ft_strlen(s);
	i = 0;
	arr = (char *) malloc(size + 1);
	if (arr == NULL)
		return (0);
	while (s[i] != '\0')
	{
		arr[i] = f(i, s[i]);
		i++;
	}
	if (s[i] == '\0')
		arr[i] = '\0';
	return (arr);
}
