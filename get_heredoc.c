/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:24:49 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/11 08:58:56 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc(t_tok *head, t_tok *tail, t_data *data)
{
	char	*name;
	int		fd;
	int err = 0;

	//sigaction(SIG, sa_ex)
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
	size_t	len;
	int		err;

	len = ft_strlen(token->word);
	err = 0;
	ft_putstr_fd("> ", 0);
	line = get_next_line(0);
	while (line && lastsignal != 2)
	{
		if (ft_strlen(line) == len + 1)
		{
			if (ft_strncmp(line, token->word, len) == 0)
				break ;
		}
		err = expand_and_write(fd, line, data);
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	else if (lastsignal != 2)
		ft_putstr_fd("\nwarning: here-document delimited by end-of-file\n", 1);
	else
		err = 2;
	close(fd);
	return (err);
}

int	expand_and_write(int fd, char *line, t_data *data)
{
	t_tok	*temp;
	int		idx;
	int		err;

	err = 0;
	temp = ft_calloc(1, sizeof(t_tok));
	if (!temp)
		return (-1);
	temp->word = ft_calloc(ft_strlen(line) + 1, sizeof(char));
	if (!temp->word)
		return (-1);
	idx = -1;
	while (line[++idx] && !err)
	{
		if (line[idx] == '$')
			idx += handle_hd_expand(&line[idx + 1], temp, data, &err);
		else
			temp->word[temp->idx++] = line[idx];
	}
	write(fd, temp->word, temp->idx);
	free(line);
	free(temp->word);
	free(temp);
	return (err);
}

int	handle_hd_expand(char *start, t_tok *token, t_data *data, int *err)
{
	char	*varvalue;
	int		i;

	i = 0;
	if (start[i] == '?' && ++i)
		varvalue = ft_itoa(data->st_code);
	else
	{
		while (start[i] && ft_isalnum(start[i]))
			i++;
		if (!i)
		{
			token->word[token->idx++] = '$';
			return (0);
		}
		varvalue = find_var(data->envp, start, i);
	}
	*err = change_word(token, varvalue, start + i, NULL);
	return (i);
}
