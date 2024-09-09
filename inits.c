/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:16:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/02 16:41:48 by auspensk         ###   ########.fr       */
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

int	new_pid(int pid, t_data *data)
{
	t_pids	*new_pid;

	new_pid = malloc(sizeof(t_pids));
	if (!new_pid)
		return (-1);
	new_pid->pid = pid;
	new_pid->next = NULL;
	if (data->pids)
	{
		while (data->pids->next)
			data->pids = data->pids->next;
		data->pids->next = new_pid;
	}
	else
		data->pids = new_pid;
	return (0);
}
