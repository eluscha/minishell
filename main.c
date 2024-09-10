/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:07:50 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 12:29:48 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	int		return_value;

	if (argc > 2)
	{
		printf ("no arguments required, only program name: %s\n", argv[0]);
		return (1);
	}

	init_data(&data, envp);
	// read_input(&data);
	data.cmd = parser(argv[1], &data);
	return_value = execute_loop(&data);
	if (data.cmd)
		clean_exit(NULL, return_value, &data);
	clean_exit(NULL, 0, &data);
}
