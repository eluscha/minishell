/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:56:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/14 14:24:03 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	clean_exit(char *msg, int r_value, t_data *data)
{
	printf("in clean exit\n");
	if (data)
	{
		data->cmd = free_cmds(data->cmd);
		data->envp = free_envp(data->envp);
		data->pids = free_pids(data->pids);
		data->paths = free_paths(data->paths);
		if (data->sa)
			free(data->sa);
		data->sa = NULL;
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	clear_history();
	printf("ready to return\n");
	return (r_value);
}

char	**free_paths(char **paths)
{
	printf("in paths \n");
	int	i;

	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	return (NULL);
}

char	**free_envp(char **envp)
{
	printf("in envp \n");
	int	i;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		free(envp[i]);
		envp[i] = NULL;
		i++;
	}
	free(envp);
	return (NULL);
}

t_cmd	*free_cmds(t_cmd *cmd_list)
{
	printf("in cmds \n");
	t_cmd	*next_node;

	while (cmd_list)
	{
		next_node = cmd_list->next;
		free_cmd(cmd_list, 0);
		cmd_list = next_node;
	}
	return (NULL);
}

t_pids	*free_pids(t_pids *pids)
{
	printf("in pids \n");
	t_pids	*next_pid;

	while (pids)
	{
		next_pid = pids->next;
		free(pids);
		pids = next_pid;
	}
	return (NULL);
}
