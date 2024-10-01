/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:33:17 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/01 14:17:49 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_error(t_cmd *cmd, char *value, t_data *data)
{
	if (cmd->cmd_check != BLTN)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	perror (value);
	data->st_code = 1;
	return (1);
}

int	duplicate_fds(int *fds, t_cmd *cmd)
{
	if (fds[0] && cmd->cmd_check != BLTN)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	if (fds[0] && cmd->cmd_check == BLTN)
		close(fds[0]);
	if (fds[1])
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
	return (0);
}

int	out_redirect(t_redirect *redirect, int *fds, t_data *data, t_cmd *cmd)
{
	if (fds[1])
		close(fds[1]);
	if (redirect->type == OUTPUT)
		fds[1] = open(redirect->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirect->type == APPEND)
		fds[1] = open(redirect->value, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (fds[1] < 0)
	{
		if (fds[0])
			close (fds[0]);
		return (redirect_error(cmd, redirect->value, data));
	}
	return (0);
}

int	in_redirect(t_redirect *redirect, int *fds, t_data *data, t_cmd *cmd)
{
	if (fds[0])
		close(fds[0]);
	fds[0] = open(redirect->value, O_RDONLY);
	if (fds[0] < 0)
	{
		if (fds[1])
			close(fds[1]);
		return (redirect_error(cmd, redirect->value, data));
	}
	return (0);
}

int	redirect(t_cmd *cmd, t_data *data)
{
	int	i;
	int	fds[2];

	i = 0;
	fds[0] = 0;
	fds[1] = 0;
	while (cmd->redirect[i].value)
	{
		if (cmd->redirect[i].type == INPUT || cmd->redirect[i].type == HEREDOC)
		{
			if (in_redirect(&(cmd->redirect[i]), fds, data, cmd) == 1)
				return (1);
		}
		if (cmd->redirect[i].type == OUTPUT || cmd->redirect[i].type == APPEND)
		{
			if (out_redirect(&(cmd->redirect[i]), fds, data, cmd) == 1)
				return (1);
		}
		i++;
	}
	duplicate_fds(fds, cmd);
	return (0);
}
