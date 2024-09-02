/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:33:17 by auspensk          #+#    #+#             */
/*   Updated: 2024/08/28 17:02:31 by auspensk         ###   ########.fr       */
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

int	out_redirect(t_cmd *cmd)
{
	int			fd;
	t_redirect	*redirect;

	if (cmd->out_redirect)
	{
		redirect = cmd->out_redirect;
		while (redirect)
		{
			if (redirect->type == 1)
				fd = open(redirect->value, O_RDWR | O_CREAT, 0644);
			else
				fd = open(redirect->value, O_CREAT | O_APPEND | O_RDWR, 0644);
			if (fd < 0)
				return (redirect_error(redirect->value));
			if (redirect->next)
				close(fd);
			redirect = redirect->next;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

int	in_redirect(t_cmd *cmd)
{
	int			fd;
	t_redirect	*redirect;

	redirect = cmd->in_redirect;
	while (redirect)
	{
		fd = open(redirect->value, O_RDONLY);
		if (fd < 0)
			return (redirect_error(redirect->value));
		if (redirect->next)
			close(fd);
		redirect = redirect->next;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redirect(t_cmd *cmd)
{
	if (cmd->in_redirect)
	{
		if (in_redirect(cmd) == 1)
			return (1);
	}
	if (cmd->out_redirect)
	{
		if (out_redirect(cmd) == 1)
			return (1);
	}
	return (0);
}
