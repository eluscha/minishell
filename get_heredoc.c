/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:24:49 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/11 13:33:10 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc(t_tok *head, t_tok *tail, t_data *data)
{
	char	*name;
	int		fd;
	int		err;
	int		pid;
	int		status = 0;

	t_tok *curr = head;
	err = 0;
	while (curr != tail && !err)
	{
		if (curr->type != HEREDOC)
		{
			curr = curr->next;
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
		signal(SIGINT, SIG_IGN);
		pid = fork();
		if (pid == 0)
			exit(get_input(fd, curr, head, data));
		close(fd);
		free(curr->word);
		curr->word = name; //change toktype ?
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			err = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			err = 1;
			lastsignal = WTERMSIG(status);
		}
		curr = curr->next;
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

int	get_input(int fd, t_tok *token, t_tok *head, t_data *data)
{
	char	*line = NULL;
	size_t	len;
	int		err;

	signal(SIGINT, SIG_DFL);
	len = ft_strlen(token->word);
	err = 0;
	while (!err)
	{
		if (line)
			free(line);
		line = NULL;
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strlen(line) == len && ft_strncmp(line, token->word, len) == 0)
			break ;
		err = expand_and_write(fd, line, data);
	}
	if (line)
		free(line);
	else
		ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
	close(fd);
	free_tokens(head);
	return (clean_exit(NULL, err, data));
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
	write(fd, "\n", 1);
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
