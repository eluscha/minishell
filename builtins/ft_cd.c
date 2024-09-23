/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:55:32 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/23 10:50:09 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	to_home(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "HOME=", 5))
		i++;
	if (data->envp[i])
		cmd->args[1] = ft_strdup(data->envp[i] + 5);
	else
		return (1);
	if (!cmd->args[1])
		return (1);
	return (0);
}

int	set_envp(char **oldpwd, t_data *data)
{
	char	*cur_dir;
	char	*pwd_dir;

	if (ft_export(*oldpwd, NULL, data))
	{
		data->st_code = 1;
		free (*oldpwd);
		return (1);
	}
	cur_dir = getcwd(NULL, 0);
	pwd_dir = ft_strjoin("PWD=", cur_dir);
	free(cur_dir);
	if (ft_export(pwd_dir, NULL, data))
	{
		data->st_code = 1;
		free(pwd_dir);
		return (1);
	}
	free(pwd_dir);
	free(*oldpwd);
	return (0);
}

void	prev_dir(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], "OLDPWD=", 7))
		{
			free(cmd->args[1]);
			cmd->args[1] = NULL;
			cmd->args[1] = ft_strdup(data->envp[i] + 7);
			write(1, cmd->args[1], ft_strlen(cmd->args[1]));
			write(1, "\n", 1);
			break ;
		}
		i++;
	}
}

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	*oldpwd;
	char	*cur_dir;

	data->st_code = 0;
	cmd->cmd_check = BLTN;
	oldpwd = NULL;
	if (redirect(cmd, data))
		return (clean_exit(NULL, 1, data));
	if (!cmd->args[1])
	{
		if (to_home(cmd, data))
			return (cd_error("HOME not set\n", cmd, data, NULL));
	}
	if (!ft_strcmp(cmd->args[1], "-"))
		prev_dir(cmd, data);
	cur_dir = getcwd(NULL, 0);
	if (!cur_dir)
		return (cd_error(NULL, cmd, data, oldpwd));
	oldpwd = ft_strjoin("OLDPWD=", cur_dir);
	free(cur_dir);
	cur_dir = NULL;
	if (!oldpwd || chdir(cmd->args[1]))
		return (cd_error(NULL, cmd, data, oldpwd));
	return (set_envp(&oldpwd, data));
}

