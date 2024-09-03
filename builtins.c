/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:31 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 14:52:32 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_cmd *cmd, t_data *data)
{
	char	*dir;

	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
	dir = getcwd(NULL, 0);
	write(1, dir, strlen(dir));
	write(1, "\n", 1);
	free(dir);
	return (1);
}

int	ft_cd(t_cmd *cmd, t_data *data)
{
	int	result;

	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
	result = chdir(cmd->args[1]);
	if (result)
	{
		write(2, "cd: ", 4);
		perror(cmd->args[1]);
		data->st_code = 1;
	}
	return (1);
}

int	check_builtin(t_cmd *cmd, t_data *data)
{
	if (!ft_strcmp(cmd->cmd, "pwd"))
		return (ft_pwd(cmd, data));
	if (!ft_strcmp(cmd->cmd, "cd"))
		return (ft_cd(cmd, data));
	if (!ft_strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd, data));
	if (!ft_strcmp(cmd->cmd, "export"))
		return (ft_export(cmd, data));
	// if (!ft_strcmp(cmd->cmd, "unset"))
	// 	return (ft_cd(cmd, data));
	// if (!ft_strcmp(cmd->cmd, "env"))
	// 	return (ft_cd(cmd, data));
	if (!ft_strcmp(cmd->cmd, "exit"))
	{
		write(1, "exit\n", ft_strlen("exit\n"));
		clean_exit(NULL, 0, data);
		exit (0);
	}
	return (0);
}
