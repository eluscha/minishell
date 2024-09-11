/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tok_fts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:25:11 by eleonora          #+#    #+#             */
/*   Updated: 2024/09/11 14:36:27 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_notpipe(t_tok *token, int cmd, int *numredir)
{
	int	err;

	err = 0;
	if (token->type >= HEREDOC)
		return (0);
	else if (token->word[0] == '<')
	{
		if (token->word[1] == '<')
			err = io_type(token, HEREDOC, numredir);
		else
			err = io_type(token, INPUT, numredir);
	}
	else if (token->word[0] == '>')
	{
		if (token->word[1] == '>')
			err = io_type(token, APPEND, numredir);
		else
			err = io_type(token, OUTPUT, numredir);
	}
	else if (!cmd)
		token->type = CMD;
	else
		token->type = ARGS;
	return (err);
}

int	io_type(t_tok *token, t_toktype type, int *numredir)
{
	char	*io_arg;
	int		idx;

	*numredir = *numredir + 1;
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

int	get_heredoc(t_tok *head, t_tok *tail, t_data *data)
{
	char	*name;
	int		fd;
	int err = 0;

	while (head != tail)
	{
		if (head->type != HEREDOC)
		{
			head = head->next;
			continue ;
		}
		name = NULL;
		fd = open_tmp_file(&name);
		if (fd == -1)
		{
			if (name)
				free(name);
			return (-1);
		}
		err = get_input(fd, head, data);
		free(head->word);
		head->word = name;
		head = head->next;
	}
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

int	get_input(int fd, t_tok *token, t_data *data)
{
	char	*line;
	int		linelen;
	int len = ft_strlen(token->word);
	int	idx;
	int err = 0;
	lex_state state = WORD;
	t_tok *temp = NULL;

	ft_putstr_fd("> ", 0);
	line = get_next_line(0);
	while (line)
	{
		linelen = ft_strlen(line);
		if (linelen == len + 1)
		{
			if (ft_strncmp(line, token->word, len) == 0)
				break ;
		}
		idx = -1;
		while(line[++idx])
		{
			if (line[idx] == '\'')
			{
				if (state == INSQTS )
					state = WORD;
				else if (state == WORD)
					state = INSQTS;
			}
			else if (state == WORD && line[idx] == '$')
			{
				temp = gen_token(0, linelen, &err);
				handle_expand(&line[idx], temp, data, &err);
				free(line);
				line = temp->word;
			}
		}
		write(fd, line, ft_strlen(line));
		free(line);
		ft_putstr_fd("> ", 0);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	if (temp)
		free(temp);
	close(fd);
	return (err);
}