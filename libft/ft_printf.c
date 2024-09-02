/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:26:14 by auspensk          #+#    #+#             */
/*   Updated: 2024/05/08 10:18:33 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	convert(char c, va_list *lst)
{
	int	count;

	count = 0;
	if (c == 'c')
		return (ft_printchar(va_arg(*lst, int)));
	else if (c == 's')
		return (ft_printstr(va_arg(*lst, char *)));
	else if (c == 'p')
		return (ft_pointer (lst, &count));
	else if (c == 'd' || c == 'i')
		return (ft_printnum(va_arg(*lst, int), &count));
	else if (c == 'u')
		return (ft_printnum(va_arg(*lst, unsigned int), &count));
	else if (c == 'x')
		return (ft_printhex(va_arg(*lst, unsigned int), 'x', &count));
	else if (c == 'X')
		return (ft_printhex(va_arg(*lst, unsigned int), 'X', &count));
	else if (c == '%')
		return (ft_printchar('%'));
	else
		return (ft_printchar(c));
}

int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	lst;

	if (format == 0 || *format == '\0')
		return (0);
	count = 0;
	va_start(lst, format);
	while (*format != '\0')
	{
		if (*format != '%')
			count += ft_printchar(*format);
		else
		{
			if (*format == ' ')
				format ++;
			count += convert(*++format, &lst);
		}
		format ++;
	}
	va_end(lst);
	return (count);
}
