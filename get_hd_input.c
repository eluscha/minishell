/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_hd_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 13:06:25 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/16 13:03:14 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hd_child(int fd, t_tok *token, t_tok *head, t_data *data)
{
	char	*line;
	char	*word;
	int		err;

	err = 0;
	line = NULL;
	word = ft_strdup(token->word);
	close(data->std_in);
	free_tokens(head);
	if (!word)
		err = -1;
	signal(SIGINT, SIG_DFL);
	if (!err)
	{
		err = get_input(&line, word, fd, data);
		if (line)
			free(line);
		else
			ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
		free(word);
	}
	close(fd);
	return (err);
}

int	get_input(char **line, char *word, int fd, t_data *data)
{
	int		err;
	size_t	len;

	err = 0;
	len = ft_strlen(word);
	while (!err)
	{
		clear_history();
		if (*line)
			free(*line);
		*line = readline("> ");
		if (!*line)
			break ;
		if (ft_strlen(*line) == len && ft_strncmp(*line, word, len) == 0)
			break ;
		err = expand_and_write(fd, *line, data);
	}
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
