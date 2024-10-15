/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:24:49 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/15 10:12:08 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc(t_tok *head, t_tok *tail, t_data *data)
{
	int		err;
	t_tok	*curr;

	err = 0;
	curr = head;
	while (!err && curr != tail)
	{
		if (curr->type != HEREDOC)
			curr = curr->next;
		else
		{
			err = hd_found(curr, head, data);
			curr = curr->next;
		}
	}
	return (err);
}

int	hd_found(t_tok *curr, t_tok *head, t_data *data)
{
	char	*name;
	int		fd;
	int		pid;
	int		err;	

	err = 0;
	name = NULL;
	fd = open_tmp_file(&name);
	if (fd == -1)
	{
		if (name)
			free(name);
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		free(name);
		err = hd_child(fd, curr, head, data);
		exit(clean_exit(NULL, err, data));
	}
	err = hd_parent(fd, pid, curr, name);
	return (err);
}

int	open_tmp_file(char **name)
{
	int			fd;
	char		*sufx;
	int			tries;
	static int	num;

	fd = -1;
	tries = 0;
	while (fd == -1 && tries++ < 100)
	{
		if (*name)
			free(*name);
		sufx = ft_itoa(num++);
		if (!sufx)
			return (-1);
		*name = ft_strjoin("/tmp/minishell_hd", sufx);
		if (!*name)
		{
			free(sufx);
			return (-1);
		}
		fd = open(*name, O_CREAT | O_EXCL | O_WRONLY | O_APPEND, 0777);
		free(sufx);
	}
	return (fd);
}

int	hd_parent(int fd, int pid, t_tok *token, char *name)
{
	int	status;
	int	err;

	err = 0;
	status = 0;
	close(fd);
	free(token->word);
	token->word = name;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		err = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		err = 1;
		lastsignal = WTERMSIG(status);
	}
	return (err);
}
