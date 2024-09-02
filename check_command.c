/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:38:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/02 15:12:20 by auspensk         ###   ########.fr       */
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

int	check_path(t_cmd *cmd)
{
	DIR	*dir;

	if (access(cmd->cmd, F_OK))
		return (3);
	dir = opendir(cmd->cmd);
	if (dir)
	{
		closedir(dir);
		return (2);
	}
	cmd->args[0] = ft_strdup(ft_strrchr(cmd->cmd, '/') + 1);
	return (0);
}

int	find_binary(t_data *data, t_cmd *cmd)
{
	char	*path;
	char	*buf;
	int		i;

	parce_paths(data);
	i = 0;
	while (data->paths[i])
	{
		buf = ft_strjoin(data->paths[i], "/");
		path = ft_strjoin(buf, cmd->cmd);
		free(buf);
		if (access(path, F_OK) == 0)
		{
			cmd->args[0] = strdup(cmd->cmd);
			free(cmd->cmd);
			cmd->cmd = strdup(path);
			free(path);
			return (0);
		}
		free(path);
		i++;
	}
	return (1);
}

int	check_command(t_cmd *cmd, t_data *data)
{
	if (check_builtin(cmd))
		return (4);
	if (ft_strchr(cmd->cmd, '/'))
		return (check_path(cmd));
	return (find_binary(data, cmd));
}
