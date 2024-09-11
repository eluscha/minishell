/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:33:17 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/10 14:26:55 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_error(char *value)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	perror (value);
	return (1);
}

int	duplicate_fds(int *fd_in, int *fd_out)
{
	if (*fd_in)
	{
		dup2(*fd_in, STDIN_FILENO);
		close(*fd_in);
	}
	if (*fd_out)
	{
		dup2(*fd_out, STDOUT_FILENO);
		close(*fd_out);
	}
	return (0);
}

int	out_redirect(t_redirect *redirect, int *fd_out, int *fd_in)
{
	if (*fd_out)
		close(*fd_out);
	if (redirect->type == OUTPUT)
		*fd_out = open(redirect->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirect->type == APPEND)
		*fd_out = open(redirect->value, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (*fd_out < 0)
	{
		if (*fd_in)
			close (*fd_in);
		return (redirect_error(redirect->value));
	}
	return (0);
}

int	in_redirect(t_redirect *redirect, int *fd_in, int *fd_out)
{
	if (*fd_in)
		close(*fd_in);
	*fd_in = open(redirect->value, O_RDONLY);
	if (*fd_in < 0)
	{
		if (*fd_out)
			close(*fd_out);
		return (redirect_error(redirect->value));
	}
	return (0);
}

int	redirect(t_cmd *cmd)
{
	int	i;
	int	fd_in;
	int	fd_out;

	i = 0;
	fd_in = 0;
	fd_out = 0;
	while (cmd->redirect[i].value)
	{
		if (cmd->redirect[i].type == INPUT || cmd->redirect[i].type == HEREDOC)
		{
			if (in_redirect(&(cmd->redirect[i]), &fd_in, &fd_out) == 1)
				return (1);
		}
		if (cmd->redirect[i].type == OUTPUT || cmd->redirect[i].type == APPEND)
		{
			if (out_redirect(&(cmd->redirect[i]), &fd_out, &fd_in) == 1)
				return (1);
		}
		i++;
	}
	duplicate_fds(&fd_in, &fd_out);
	return (0);
}
