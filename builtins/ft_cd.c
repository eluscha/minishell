/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:55:32 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/11 13:57:22 by auspensk         ###   ########.fr       */
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

