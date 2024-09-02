/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:26:33 by auspensk          #+#    #+#             */
/*   Updated: 2024/05/08 10:19:45 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printchar(char c)
{
	write (1, &c, 1);
	return (1);
}

int	ft_printstr(char *str)
{
	int	i;

	i = 0;
	if (str == 0)
		return (ft_printstr("(null)"));
	else
	{
		while (str[i] != '\0')
		{
			write(1, &str[i], 1);
			i++;
		}
	}
	return (i);
}

int	ft_printnum(long long int num, int *count)
{
	if (num < 0)
	{
		num = num * -1;
		*count = ft_printchar('-');
	}
	if (num > 9)
	{
		ft_printnum(num / 10, count);
		ft_printnum(num % 10, count);
	}
	else
		*count += ft_printchar(num % 10 + '0');
	return (*count);
}

int	ft_pointer(va_list *lst, int *count)
{
	void	*ptr;

	ptr = va_arg(*lst, void *);
	if (ptr == 0)
		return (ft_printstr("(nil)"));
	*count = ft_printstr("0x");
	return (ft_printhex((unsigned long)ptr, 'x', count));
}

int	ft_printhex(unsigned long int num, char x, int *count)
{
	char		*hex1;
	char		*hex2;

	hex1 = "0123456789abcdef";
	hex2 = "0123456789ABCDEF";
	if (num < 16)
	{
		if (x == 'x')
			*count += ft_printchar (hex1[num]);
		else
			*count += ft_printchar (hex2[num]);
	}
	else
	{
		ft_printhex(num / 16, x, count);
		ft_printhex(num % 16, x, count);
	}
	return (*count);
}
