/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 13:56:59 by auspensk          #+#    #+#             */
/*   Updated: 2024/04/28 13:57:13 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*first;
	t_list	*curr;

	if (lst == 0 || f == 0 || del == 0)
		return (0);
	curr = lst;
	first = ft_lstnew(f(curr -> content));
	if (first == 0)
		return (0);
	while (curr ->next != 0)
	{
		curr = curr -> next;
		new = ft_lstnew(f(curr -> content));
		if (new == 0)
		{
			ft_lstclear(&first, del);
			return (0);
		}
		ft_lstadd_back(&first, new);
	}
	return (first);
}
