/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tok_fts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:25:11 by eleonora          #+#    #+#             */
/*   Updated: 2024/09/10 11:48:39 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_notpipe(t_tok *token, int cmd)
{
	int	err;

	err = 0;
	if (token->type >= HEREDOC)
		return (0);
	else if (token->word[0] == '<')
	{
		if (token->word[1] == '<')
			err = heredoc(token);
		else
			err = io_type(token, INPUT);
	}
	else if (token->word[0] == '>')
	{
		if (token->word[1] == '>')
			err = io_type(token, APPEND);
		else
			err = io_type(token, OUTPUT);
	}
	else if (!cmd)
		token->type = CMD;
	else
		token->type = ARGS;
	return (err);
}

int	io_type(t_tok *token, t_toktype type)
{
	char	*io_arg;
	int		idx;

	token->type = type;
	if (type == APPEND || type == HEREDOC)
		idx = 2;
	else
		idx = 1;
	if (!token->word[idx])
	{
		token->type = DISCARD;
		if (token->next->type != END)
			token->next->type = type;
	}
	else
	{
		token->type = type;
		io_arg = ft_strdup(token->word + idx);
		if (!io_arg)
			return (-1);
		free(token->word);
		token->word = io_arg;
	}
	return (0);
}

int	heredoc(t_tok *token)
{
	char	*name;
	int		fd;

	if (io_type(token, HEREDOC) == -1)
		return (-1);
	name = NULL;
	fd = open_tmp_file(&name);
	if (fd == -1)
	{
		if (name)
			free(name);
		return (-1);
	}
	get_input(fd, token->word, ft_strlen(token->word));
	close(fd);
	free(token->word);
	token->word = name;
	return (0);
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

void	get_input(int fd, char *limiter, size_t len)
{
	char	*line;

	ft_putstr_fd("> ", 0);
	line = get_next_line(0);
	while (line)
	{
		if (ft_strlen(line) == len + 1)
		{
			if (ft_strncmp(line, limiter, len) == 0)
				break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
		ft_putstr_fd("> ", 0);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	return ;
}