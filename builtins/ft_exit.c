/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:08:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/09 10:29:29 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	err_exit(char *str, t_data *data)
{
	write(2, "exit: ", ft_strlen("exit: "));
	write(2, str, ft_strlen(str));
	write(2, ": numeric argument required\n",
		ft_strlen(": numeric argument required\n"));
	close(data->std_in);
	exit (clean_exit(NULL, 2, data));
}

void	parse_ex_status(char *str, t_data *data)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	if (str[i] == '\0' || ft_strlen(str) > 18)
		err_exit(str, data);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] > 47 && *str < 58)
			status = (status * 10) + (str[i] - 48);
		else
			err_exit(str, data);
		i++;
	}
	while (status > 255)
		status = status % 256;
	if (*str == '-')
		status = 256 - status;
	close(data->std_in);
	exit (clean_exit (NULL, status, data));
}

void	ft_exit(t_cmd *cmd, t_data *data)
{
	cmd->cmd_check = BLTN;
	if (redirect(cmd, data))
	{
		clean_exit(NULL, data->st_code, data);
		return ;
	}
	if (!data->child)
		write(1, "exit\n", ft_strlen("exit\n"));
	if (cmd->args[2])
	{
		write(2, "exit: too many arguments\n", 25);
		data->st_code = 1;
		return ;
	}
	if (cmd->args[1])
		return (parse_ex_status(cmd->args[1], data));
	close(data->std_in);
	exit (clean_exit(NULL, data->st_code, data));
}
