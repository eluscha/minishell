/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:16:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/08/28 16:47:29 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data *data, char **envp)
{
	data->pids = NULL;
	data->cmd = NULL;
	data->pids = NULL;
	data->envp = envp;
	data->paths = NULL;
	data->tty_in = ttyname(STDIN_FILENO);
	data->tty_out = ttyname(STDOUT_FILENO);
	data->st_code = 0;
}
