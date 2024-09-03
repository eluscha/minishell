/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:07:40 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 12:13:16 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (!strcmp(cmd->args[1], "-n"))
			write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		else
		{
			write(1, cmd->args[i], ft_strlen(cmd->args[i]));
			write(1, "\n", 1);
		}
	}
	return (1);
}
