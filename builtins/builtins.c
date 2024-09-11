/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:31 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/11 15:27:53 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_cmd *cmd, t_data *data)
{
	char	*dir;

	data->st_code = 0;
	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
	dir = getcwd(NULL, 0);
	write(1, dir, strlen(dir));
	write(1, "\n", 1);
	free(dir);
	return (1);
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	int		i;

	data->st_code = 0;
	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
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
		return (ft_export(cmd->args[1], cmd, data));
	if (!ft_strcmp(cmd->cmd, "unset"))
		return (ft_unset(cmd, data));
	if (!ft_strcmp(cmd->cmd, "env"))
		return (print_array(data->envp));
	if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(cmd, data);
	return (0);
}
