/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 10:38:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/15 09:46:28 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_not_found(t_cmd *cmd, t_data *data)
{
	if (cmd->cmd_check == CMDNF)
	{
		write(2, "Command \'", ft_strlen("Command \'"));
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, "\' not found\n", strlen("\' not found\n"));
		data->st_code = 127;
	}
	if (cmd->cmd_check == ISDIR)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": Is a directory\n", ft_strlen(": Is a directory\n"));
		data->st_code = 126;
	}
	if (cmd->cmd_check == NSCHFL)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": No such file or directory\n",
			ft_strlen(": No such file or directory\n"));
		data->st_code = 127;
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(data->std_in);
	exit (clean_exit(NULL, data->st_code, data));
}

int	parce_paths(t_data *data, t_cmd *cmd)
{
	int		i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH=", 5))
		i++;
	if (!data->envp[i])
	{
		cmd->cmd_check = NSCHFL;
		data->paths = NULL;
		return (1);
	}
	data->paths = ft_split(data->envp[i] + 5, ':');
	return (0);
}

int	check_path(t_cmd *cmd)
{
	DIR	*dir;

	if (access(cmd->cmd, F_OK))
		cmd->cmd_check = NSCHFL;
	else
	{
		dir = opendir(cmd->cmd);
		if (dir)
		{
			closedir(dir);
			cmd->cmd_check = ISDIR;
		}
		else
			cmd->args[0] = ft_strdup(ft_strrchr(cmd->cmd, '/') + 1);
	}
	return (0);
}

int	find_binary(t_data *data, t_cmd *cmd)
{
	char	*path;
	char	*buf;
	int		i;

	if (parce_paths(data, cmd))
		return (1);
	i = 0;
	while (data->paths[i])
	{
		buf = ft_strjoin(data->paths[i], "/");
		path = ft_strjoin(buf, cmd->cmd);
		free(buf);
		if (access(path, F_OK) == 0)
		{
			cmd->args[0] = ft_strdup(cmd->cmd);
			free(cmd->cmd);
			cmd->cmd = NULL;
			cmd->cmd = path;
			return (0);
		}
		free(path);
		i++;
	}
	cmd->cmd_check = CMDNF;
	return (1);
}

int	check_command(t_cmd *cmd, t_data *data)
{
	int	i;

	if (ft_strchr(cmd->cmd, '/'))
		return (check_path(cmd));
	if (*cmd->cmd == '\0')
	{
		cmd->cmd_check = CMDNF;
		return (1);
	}
	i = find_binary(data, cmd);
	data->paths = free_paths(data->paths);
	return (i);
}
