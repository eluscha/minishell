/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:54:48 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/02 16:41:53 by auspensk         ###   ########.fr       */
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

void	free_pids(t_data *data)
{
	t_pids	*next_pid;

	while (data->pids)
	{
		next_pid = data->pids->next;
		free (data->pids);
		data->pids = next_pid;
	}
}

int	clean_exit(char *msg, int r_value, t_data *data)
{
	t_cmd	*next_node;
	t_cmd	*cmd_list;

	if (data)
	{
		cmd_list = data->cmd;
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
		free_pids(data);
		data->cmd = NULL;
	}

	if (msg)
		write(2, msg, ft_strlen(msg));
	return (r_value);
}

