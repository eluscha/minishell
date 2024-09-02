/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 13:57:54 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/28 13:58:07 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*curr;
	size_t	size;

	if (lst == 0)
		return (0);
	curr = lst;
	size = 1;
	while (curr -> next != 0)
	{
		curr = curr -> next;
		size ++;
	}
	return (size);
}
