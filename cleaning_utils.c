/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:54:48 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/09 11:38:01 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_paths(t_data *data)
{
	int	i;

	i = 0;
	if (!data->paths)
		return ;
	while (data->paths[i])
	{
		free(data->paths[i]);
		i++;
	}
	free(data->paths);
	data->paths = NULL;
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		envp[i] = NULL;
		i++;
	}
	free(envp);
}

void	free_cmds(t_cmd	*cmd_list)
{
	t_cmd	*next_node;

	while (cmd_list)
	{
		next_node = cmd_list->next;
		free_cmd(cmd_list, 0);
		cmd_list = next_node;
	}
}

void	free_pids(t_data *data)
{
	t_pids	*next_pid;

	while (data->pids)
	{
		next_pid = data->pids->next;
		free(data->pids);
		data->pids = NULL;
		data->pids = next_pid;
	}
}

void	free_sas(t_data *data)
{
	if (data->sa)
		free(data->sa);
	if (data->sa_child)
		free(data->sa_child);
	if (data->sa_ex)
		free(data->sa_ex);
	if (data->sa_quit)
		free(data->sa_quit);
	if (data->sa_quit_child)
		free(data->sa_quit_child);
	if (data->sa_quit_ex)
		free(data->sa_quit_ex);
	data->sa = NULL;
	data->sa_child = NULL;
	data->sa_ex = NULL;
	data->sa_quit = NULL;
	data->sa_quit_child = NULL;
	data->sa_quit_ex = NULL;
}
