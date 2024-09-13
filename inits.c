/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:16:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/13 11:29:11 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **dup_envp(char **envp)
{
	int		i;
	char	**dup_envp;

	i = 0;
	dup_envp = ft_calloc(100, sizeof(char *));
	if (!dup_envp)
		return (NULL);
	while (envp[i])
	{
		dup_envp[i] = ft_strdup(envp[i]);
		if (!dup_envp[i])
		{
			while (--i >= 0)
				free(dup_envp[i]);
			free(dup_envp);
			return (NULL);
		}
		i++;
	}
	return (dup_envp);
}

void	init_data(t_data *data, char **envp)
{
	data->pids = NULL;
	data->cmd = NULL;
	data->pids = NULL;
	data->envp = dup_envp(envp);
	data->paths = NULL;
	data->tty_in = ttyname(STDIN_FILENO);
	data->tty_out = ttyname(STDOUT_FILENO);
	data->st_code = 0;
	if (!data->envp)
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
