/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:38:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/02 10:18:57 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parce_paths(t_data *data)
{
	int		i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH=", 5))
		i++;
	data->paths = ft_split(data->envp[i] + 5, ':');
}

int	get_first_arg(t_cmd *cmd)
{
	if (*(cmd->cmd) == '/')
		cmd->args[0] = ft_strdup(ft_strrchr(cmd->cmd, '/') + 1);
	else
		cmd->args[0] = ft_strdup(cmd->cmd);
	return (0);
}

int	concatenate_path(t_data *data, t_cmd *cmd)
{
	char	*path;
	char	*buf;
	int		i;

	i = 0;
	while (data->paths[i])
	{
		buf = ft_strjoin(data->paths[i], "/");
		path = ft_strjoin(buf, cmd->cmd);
		free(buf);
		if (access(path, F_OK) == 0)
		{
			cmd->args[0] = cmd->cmd;
			free(cmd->cmd);
			cmd->cmd = strdup(path);
			return (0);
		}
		free(path);
		i++;
	}
	return (1);
}

int	get_path(t_cmd *cmd, t_data *data)
{
	parce_paths(data);
	if (!check_builtin(cmd) && access(cmd->cmd, F_OK))
	{
		return (concatenate_path(data, cmd));
	}
	return (get_first_arg(cmd));
}
