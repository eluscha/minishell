/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:31 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/11 13:33:38 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	cd_error(char *msg, t_cmd *cmd, t_data *data, char *oldpwd)
{
	write(2, "cd: ", 4);
	if (msg)
		write(2, msg, ft_strlen(msg));
	else
		perror(cmd->args[1]);
	data->st_code = 1;
	if (oldpwd)
		free(oldpwd);
	return (1);
}

int	ft_cd(t_cmd *cmd, t_data *data)
{
	int		result;
	int		i;
	char	*oldpwd;

	i = 0;
	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
	if (!cmd->args[1])
	{
		while (data->envp[i] && ft_strncmp(data->envp[i], "HOME=", 5))
			i++;
		if (data->envp[i])
			cmd->args[1] = ft_strdup(data->envp[i] + 5);
		else
			return (cd_error("HOME not set\n", cmd, data, NULL));
	}
	oldpwd = ft_strjoin("OLDPWD=", getcwd(NULL, 0));
	result = chdir(cmd->args[1]);
	if (result)
		return (cd_error(NULL, cmd, data, oldpwd));
	ft_export(oldpwd, NULL, data);
	ft_export(ft_strjoin("PWD=", getcwd(NULL, 0)), NULL, data);
	free(oldpwd);
	return (1);
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	int		i;

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
	{
		write(1, "exit\n", ft_strlen("exit\n"));
		clean_exit(NULL, 0, data);
		exit (0);
	}
	return (0);
}
