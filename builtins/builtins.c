/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:31 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/27 15:04:15 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_cmd *cmd, t_data *data)
{
	char	*dir;

	cmd->cmd_check = BLTN;
	data->st_code = 0;
	if (redirect(cmd, data))
		return ;
	dir = getcwd(NULL, 0);
	if (!dir)
	{
		data->st_code = 1;
		return ;
	}
	write(1, dir, strlen(dir));
	write(1, "\n", 1);
	free(dir);
	dir = NULL;
	return ;
}

void	ft_envp(t_cmd *cmd, t_data *data)
{
	cmd->cmd_check = BLTN;
	data->st_code = 0;
	if (redirect(cmd, data))
		return ;
	print_array(data->envp);
	return ;
}

int	set_output(t_cmd *cmd, t_data *data)
{
	int	tty_fd;

	if (cmd->cmd_check == BLTN)
	{
		tty_fd = open(data->tty_out, O_RDWR, O_APPEND);
		dup2(tty_fd, STDOUT_FILENO);
		close(tty_fd);
		return (1);
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
	{
		if (ft_export(cmd->args[1], cmd, data))
			data->st_code = 1;
	}
	else if (!ft_strcmp(cmd->cmd, "unset"))
	{
		if (ft_unset(cmd, data))
			data->st_code = 1;
	}
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_envp(cmd, data);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(cmd, data);
	return (set_output (cmd, data));
}
