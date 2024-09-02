/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:31 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/02 14:24:49 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*dir;

	fflush(stdout);
	dir = getcwd(NULL, 0);
	write(1, dir, strlen(dir));
	write(1, "\n", 1);
	free(dir);
	return (1);
}

int	check_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->cmd, "pwd"))
		return (ft_pwd());
	return (0);
	// if (!ft_strncmp(cmd->cmd, "echo", 4))
	// 	ft_echo(cmd);
	// if (!ft_strncmp(cmd->cmd, "cd", 4))
	// 	ft_cd(cmd);
	// if (!ft_strncmp(cmd->cmd, "pwd", 4))
	// 	ft_pwd();
}
