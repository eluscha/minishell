/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/14 12:30:39 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_echo(t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!ft_strcmp(cmd->args[1], "-n"))
		i = 2;
	while (cmd->args[i + 1])
	{
		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		write(1, " ", 1);
		i++;
	}
	if (cmd->args[i])
		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
	if (ft_strcmp(cmd->args[1], "-n"))
		write(1, "\n", 1);
}

void	ft_echo(t_cmd *cmd, t_data *data)
{
	cmd->cmd_check = BLTN;
	data->st_code = 0;
	if (redirect(cmd, data))
		return ;
	if (data->child)
	{
		dup2(data->std_in, STDIN_FILENO);
		close (data->std_in);
	}
	if (!cmd->args[1])
		write(1, "\n", 1);
	else
		write_echo(cmd);
	return ;
}
