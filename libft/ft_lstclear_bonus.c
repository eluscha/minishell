/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 13:54:32 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/28 13:55:12 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*curr;
	t_list	*buf;

	curr = *lst;
	if (lst == 0 || *lst == 0 || del == 0)
		return ;
	while (curr != 0)
	{
		del(curr -> content);
		buf = curr -> next;
		free(curr);
		curr = buf;
	}
	*lst = 0;
}
