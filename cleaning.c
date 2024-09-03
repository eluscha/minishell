/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:54:48 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 16:17:52 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	free_redirects(t_redirect *list)
{
	t_redirect	*next_node;

	while (list)
	{
		// if (list->value)
		// 	free(list->value);
		next_node = list->next;
		free(list);
		list = next_node;
	}
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
		i++;
	}
}

void	free_cmds(t_cmd	*cmd_list)
{
	t_cmd	*next_node;

	while (cmd_list)
	{
		if (cmd_list->cmd)
			free(cmd_list->cmd);
		if (cmd_list->args)
			free(cmd_list->args);
		if (cmd_list->in_redirect)
			free_redirects(cmd_list->in_redirect);
		if (cmd_list->out_redirect)
			free_redirects(cmd_list->out_redirect);
		next_node = cmd_list->next;
		free(cmd_list);
		cmd_list = next_node;
	}
}

int	clean_exit(char *msg, int r_value, t_data *data)
{
	t_pids	*next_pid;

	if (data)
	{
		free_cmds(data->cmd);
		data->cmd = NULL;
		free_envp(data->envp);
		data->envp = NULL;
		while (data->pids)
		{
			next_pid = data->pids->next;
			free (data->pids);
			data->pids = next_pid;
		}
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	return (r_value);
}

