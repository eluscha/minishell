/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:25:11 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/25 12:02:49 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_tokens(t_tok *token, int *numargs, int *numredir)
{
	int	cmd;
	int	err;

	cmd = 0;
	*numargs = 0;
	*numredir = 0;
	err = 0;
	while (token->type != END)
	{
		if (token->word[0] == '|')
		{
			token->type = PIPE;
			cmd = 0;
		}
		else
			err = handle_notpipe(token, cmd, numredir);
		if (token->type == CMD)
			cmd = 1;
		else if (token->type == ARGS)
			*numargs = *numargs + 1;
		token = token->next;
	}
	return (err);
}

int	handle_notpipe(t_tok *token, int cmd, int *numredir)
{
	int	err;

	err = 0;
	if (token->type >= HEREDOC)
		return (0);
	else if (token->type != NOSPECIAl && token->word[0] == '<')
	{
		if (token->word[1] == '<')
			err = io_type(token, HEREDOC, numredir);
		else
			err = io_type(token, INPUT, numredir);
	}
	else if (token->type != NOSPECIAl && token->word[0] == '>')
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
	{
		idx = 2;
		if (token->word[idx] == '-'
			&& !token->word[idx + 1] && type == HEREDOC)
			idx = 3;
	}
	else
		idx = 1;
	if (!token->word[idx])
	{
		token->type = DISCARD;
		if (token->next->type != END)
			token->next->type = type;
		return (0);
	}
	if (type == HEREDOC && token->word[2] == '-')
		return (handle_hddash(token));
	token->type = type;
	io_arg = ft_strdup(token->word + idx);
	if (!io_arg)
		return (-1);
	free(token->word);
	token->word = io_arg;
	return (0);
}

int handle_hddash(t_tok *token)
{
	int idx = 3;
	if (!token->word[idx])
	{
		token->type = DISCARD;
		if (token->next->type != END)
			token->next->type = HDDASH;
		return (0);
	}
	token->type = HDDASH;
	char *arg = ft_strdup(token->word + idx);
	if (!arg)
		return (-1);
	free(token->word);
	token->word = arg;
	return (0);
}
