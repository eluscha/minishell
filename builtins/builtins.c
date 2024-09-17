/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:31 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/17 10:28:55 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_cmd *cmd, t_data *data)
{
	char	*dir;

	cmd->cmd_check = BLTN;
	data->st_code = 0;
	if (redirect(cmd, data))
		return (1);
	dir = getcwd(NULL, 0);
	if (!dir)
	{
		data->st_code = 1;
		return (1);
	}
	write(1, dir, strlen(dir));
	write(1, "\n", 1);
	free(dir);
	return (0);
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	int		i;

	cmd->cmd_check = BLTN;
	data->st_code = 0;
	if (redirect(cmd, data))
		return (1);
	i = 1;
	if (!cmd->args[1])
		write(1, "\n", 1);
	else
	{
		if (!strcmp(cmd->args[1], "-n"))
			i = 2;
		while (cmd->args[i + 1])
		{
			write(1, cmd->args[i], ft_strlen(cmd->args[i]));
			write(1, " ", 1);
			i++;
		}
		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		if (strcmp(cmd->args[1], "-n"))
			write(1, "\n", 1);
	}
	return (0);
}



int	check_builtin(t_cmd *cmd, t_data *data)
{
	if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd(cmd, data);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(cmd, data);
	else if (!ft_strcmp(cmd->cmd, "echo"))
		ft_echo(cmd, data);
	else if (!ft_strcmp(cmd->cmd, "export"))
		ft_export(cmd->args[1], cmd, data);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		ft_unset(cmd, data);
	else if (!ft_strcmp(cmd->cmd, "env"))
		print_array(data->envp);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(cmd, data);
	if (cmd->cmd_check == BLTN)
		return (1);
	else
		return (0);
}
