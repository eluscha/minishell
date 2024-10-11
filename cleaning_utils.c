/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:54:48 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/11 13:16:31 by eusatiko         ###   ########.fr       */
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
		//data->pids = NULL;
		data->pids = next_pid;
	}
}
