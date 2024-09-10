/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:55:41 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/10 12:02:04 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_input(t_data *data)
{
	char	*input;

	while (1)
	{
		input = readline("minishell:");
		if (!input)
			break ;
		if (*input)
		{
			data->cmd = parce();
			execute_loop(data);
		}
		free (input);
	}
}
