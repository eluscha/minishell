/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:08:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/17 15:46:29 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	err_exit(char *str, t_data *data)
{
	write(2, "exit: ", ft_strlen("exit: "));
	write(2, str, ft_strlen(str));
	write(2, ": numeric argument required\n",
		ft_strlen(": numeric argument required\n"));
	data->st_code = 2;
	return (0);
}

int	parse_ex_status(char *str, t_data *data)
{
	int	status;
	int	sign;

	status = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str != '\0')
	{
		if (*str > 47 && *str < 58)
			status = (status * 10) + (*str - 48);
		else
			return (err_exit(str, data));
		str++;
	}
	clean_exit(NULL, 0, data);
	exit (status);
}

int	ft_exit(t_cmd *cmd, t_data *data)
{
	cmd->cmd_check = BLTN;
	data->st_code = 0;
	write(1, "exit\n", ft_strlen("exit\n"));
	if (cmd->args[1])
		return (parse_ex_status(cmd->args[1], data));
	clean_exit(NULL, 0, data);
	exit (0);
}
