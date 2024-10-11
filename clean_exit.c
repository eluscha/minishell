/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:56:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/11 13:30:34 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	clean_exit(char *msg, int r_value, t_data *data)
{
	if (data)
	{
		free_cmds(data->cmd);
		data->cmd = NULL;
		free_envp(data->envp);
		data->envp = NULL;
		free_pids(data);
		data->pids = NULL;
		if (data->sa)
			free(data->sa);
		data->sa = NULL;
		free_paths(data);
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	clear_history();
	return (r_value);
}
