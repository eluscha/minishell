/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:18:57 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/11 14:11:42 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parser(t_tok *head, t_data *data)
{
	int		numargs;
	int		numredir;
	t_tok	*tail;
	t_cmd	*cmds;

	numargs = 0;
	numredir = 0;
	process_tokens(head, &numargs, &numredir);
	tail = check_syntax(head);
	if (tail->type != END || get_heredoc(head, tail, data) != 0)
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
	int		i;
	t_lex	*lex;

	lex = init_lex(input, tail, data);
	if (!lex)
		return (NULL);
	i = -1;
	while (input[++i] && !lex->err)
	{
		if (*lex->state != INSQTS && *lex->state != INDQTS)
			check_word_border(lex, input[i]);
		if (input[i] == '\'' || input[i] == '\"')
			handle_quotes(lex, input[i]);
		else if (input[i] == '|' || input[i] == '>' || input[i] == '<')
			handle_special(lex, input[i]);
		else if (input[i] == '$')
			i += handle_expand(lex, i + 1);
		else if (*lex->state != DELIM)
			lex->tail->word[lex->tail->idx++] = input[i];
	}
	tail = set_end(lex);
	free(lex);
	return (tail);
}

t_tok	*check_syntax(t_tok *head)
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
			printf("syntax error near unexpected token `");
			printf("%s\'\n", head->word);
			break ;
		}
	}
	return (head);
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
