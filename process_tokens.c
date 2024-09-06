/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:25:11 by eleonora          #+#    #+#             */
/*   Updated: 2024/09/06 10:52:53 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_tokens(t_tok *token)
{
	int	cmd;
	int	args;
	int	err;

	cmd = 0;
	args = 0;
	err = 0;
	while (token->type != END)
	{
		if (token->word[0] == '|')
			err = handle_pipe(token, &cmd);
		else
			err = handle_notpipe(token, cmd);
		if (token->type == CMD)
			cmd = 1;
		else if (token->type == ARGS)
			args++;
		token = token->next;
	}
	if (err)
		return (err);
	return (args);
}

int	handle_pipe(t_tok *token, int *cmd)
{
	char	*word;
	int		err;
	t_tok	*next;

	err = 0;
	token->type = PIPE;
	*cmd = 0;
	if (!token->word[1])
		return (0);
	word = ft_strdup(token->word + 1);
	if (!word)
		return (-1);
	next = token->next;
	token->next = gen_token(UNDETERM, ft_strlen(word), &err);
	if (err)
	{
		free(word);
		token->next = next;
		return (-1);
	}
	token->next->word = word;
	token->next->next = next;
	token->word[1] = '\0';
	return (0);
}

int	handle_notpipe(t_tok *token, int cmd)
{
	int	err;

	err = 0;
	if (token->type >= HEREDOC)
		return (0);
	else if (token->word[0] == '<')
	{
		if (token->word[1] == '<')
			err = io_type(token, HEREDOC);
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
	if (type == HEREDOC || type == APPEND)
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
