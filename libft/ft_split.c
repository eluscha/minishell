/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 09:45:54 by auspensk          #+#    #+#             */
/*   Updated: 2024/05/13 10:33:18 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_arr(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i --;
	}
	free(arr);
	return (0);
}

static char	**fill_arr(char **arr, char const *s, char c)
{
	size_t	i_end;
	size_t	i_start;
	size_t	i_ar;

	i_end = 0;
	i_start = 0;
	i_ar = 0;
	while (i_end <= ft_strlen(s))
	{
		while (s[i_start] == c)
			i_start ++;
		if ((s[i_end] == c || s[i_end] == '\0') && i_start < i_end)
		{
			arr[i_ar] = (char *) malloc(i_end + 1 - i_start);
			if (arr[i_ar] == 0)
				return (free_arr(arr, i_ar));
			ft_strlcpy(arr[i_ar], &s[i_start], i_end + 1 - i_start);
			i_ar++;
			i_start = ++i_end;
		}
		else
			i_end++;
	}
	arr[i_ar] = NULL;
	return (arr);
}

static int	ft_arsize(char const *s, char c)
{
	int	arsize;
	int	i;

	arsize = 1;
	i = 1;
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i - 1] != c)
			arsize++;
		i++;
	}
	if (i > 0 && s[i] == '\0' && s[i -1] != c)
		arsize++;
	return (arsize);
}

char	**ft_split(char const *s, char c)
{
	int			arsize;
	char		**arr;
	char		**res;

	if (s == 0 || *s == '\0' || *s == 0)
	{
		res = (char **) malloc(sizeof(char **));
		if (res == 0)
			return (0);
		res[0] = NULL;
		return (res);
	}
	arsize = ft_arsize(s, c);
	arr = (char **) malloc(arsize * sizeof(char **));
	if (arr == NULL)
		return (NULL);
	res = fill_arr(arr, s, c);
	if (res == NULL)
		return (0);
	return (res);
}
