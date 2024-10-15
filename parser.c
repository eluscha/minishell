/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:18:57 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/15 10:16:10 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parser(t_tok *head, t_data *data)
{
	int		numargs;
	int		numredir;
	t_tok	*end;
	t_cmd	*cmds;
	int		syntax_err;

	numargs = 0;
	numredir = 0;
	process_tokens(head, &numargs, &numredir);
	syntax_err = check_syntax(head, &end, data);
	if (get_heredoc(head, end, data) != 0 || syntax_err)
	{
		if (lastsignal)
		{
			data->st_code = lastsignal + 128;
			lastsignal = 0;
		}
		cmds = NULL;
	}
	else
		cmds = generate_structs(head, numargs, numredir);
	free_tokens(head);
	return (cmds);
}

t_tok	*lexer(char *input, t_tok *tail, t_data *data)
{
	t_lex	*lex;
	int		i;
	char	c;

	lex = init_lex(input, tail, data);
	if (!lex)
		return (NULL);
	i = -1;
	while (input[++i] && !lex->err)
	{
		c = input[i];
		if (*lex->state != INSQTS && *lex->state != INDQTS)
			check_word_border(lex, c);
		if (c == '\'' || c == '\"')
			handle_quotes(lex, c);
		else if (c == '|' || c == '>' || c == '<')
			handle_special(lex, c);
		else if (c == '$')
			i += handle_expand(lex, i + 1);
		else if (*lex->state != DELIM)
			lex->tail->word[lex->tail->idx++] = c;
	}
	tail = set_end(lex);
	free(lex);
	return (tail);
}

int		check_syntax(t_tok *head, t_tok **end, t_data *data)
{
	t_toktype	ntype;
	int			err;

	err = 0;
	while (head->type != END)
	{
		ntype = head->next->type;
		if (head->type == PIPE && ntype != CMD && ntype < IOTYPE)
			err = 1;
		else if (head->type == IOTYPE && ntype < HEREDOC)
			err = 1;
		head = head->next;
		if (err)
		{
			data->st_code = 2;
			printf("syntax error near unexpected token `");
			printf("%s\'\n", head->word);
			break ;
		}
	}
	*end = head;
	return (err);
}

int	multi_pipe_check(t_tok *head)
{
	t_toktype	ntype;

	while (head->next)
	{
		ntype = head->next->type;
		if (head->type == PIPE && ntype == PIPE)
			return (1);
		head = head->next;
	}
	return (0);
}

t_tok	*free_tokens(t_tok *head)
{
	t_tok	*ptr;

	while (head)
	{
		if (head->type >= END && head->type <= PIPERR)
			break ;
		if (head->word)
			free(head->word);
		ptr = head->next;
		free(head);
		head = ptr;
	}
	if (head)
	{
		if (head->word)
			free(head->word);
		free(head);
	}
	return (NULL);
}
