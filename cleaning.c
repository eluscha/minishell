/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:54:48 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/23 10:46:41 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	free_redirects(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->redirect[i]. value)
	{
		if (cmd->redirect[i].value)
			free(cmd->redirect[i].value);
		i++;
	}
	free(cmd->redirect);
}
*/

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
	envp = NULL; //added this line
}

void	free_cmds(t_cmd	*cmd_list)
{
	t_cmd	*next_node;

	while (cmd_list)
	{
		next_node = cmd_list->next;
		free_cmd(cmd_list, 0); //changed to my function, now this is so short can be maybe unuted with clean_exit
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

int	clean_exit(char *msg, int r_value, t_data *data)
{
	if (data)
	{
		free_cmds(data->cmd);
		data->cmd = NULL;
		free_envp(data->envp);
		data->envp = NULL;
		free_pids(data);
		if (data->sa)
			free(data->sa);
		if (data->sa_child)
			free(data->sa_child);
		if (data->sa_ex)
			free(data->sa_ex);
		if (data->sa_quit)
			free(data->sa_quit);
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	clear_history();
	return (r_value);
}

