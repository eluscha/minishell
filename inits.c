/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:16:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 16:35:04 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_envp(char **envp, t_data *data)
{
	int		i;

	i = 0;
	data->envp = malloc(sizeof(char **) * 100);
	if (!data->envp)
		return ;
	while (envp[i])
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	data->envp[i] = NULL;
	return ;
}

void	init_data(t_data *data, char **envp)
{
	data->pids = NULL;
	data->cmd = NULL;
	data->pids = NULL;
	dup_envp(envp, data);
	data->paths = NULL;
	data->tty_in = ttyname(STDIN_FILENO);
	data->tty_out = ttyname(STDOUT_FILENO);
	data->st_code = 0;
	if (!data->envp || !data->tty_in || !data->tty_out)
	{
		clean_exit("failed to init data\n", 1, data);
		exit (1);
	}
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
