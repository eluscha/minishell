/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mid_fts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:19:14 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/14 10:57:20 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_word_border(t_lex *lex, char c)
{
	if (c == ' ')
	{
		if (*lex->state == DELIM || *lex->state == EXPAND)
			*lex->state = DELIM;
		else
		{
			*lex->state = DELIM;
			lex->tail->next = gen_token(UNDETERM, 0, lex);
			if (!lex->err)
				lex->tail = lex->tail->next;
		}
		return ;
	}
	else if (*lex->state == INREDIR && lex->tail->idx == 1 && c == '<')
		return ;
	else if (*lex->state == OUTREDIR && lex->tail->idx == 1 && c == '>')
		return ;
	else if (*lex->state >= INREDIR || (c == '|' && lex->tail->idx))
	{
		lex->tail->next = gen_token(UNDETERM, 0, lex);
		if (!lex->err)
			lex->tail = lex->tail->next;
	}
	*lex->state = WORD;
	lex->lastchar = c;
}

void	handle_quotes(t_lex *lex, char c)
{
	if (c == '\'')
	{
		if (*lex->state == INSQTS)
			*lex->state = WORD;
		else if (*lex->state == WORD)
			*lex->state = INSQTS;
		else
			lex->tail->word[lex->tail->idx++] = c;
	}
	else if (c == '\"')
	{
		if (*lex->state == INDQTS)
			*lex->state = WORD;
		else if (*lex->state == WORD)
			*lex->state = INDQTS;
		else
			lex->tail->word[lex->tail->idx++] = c;
	}
}

void	handle_special(t_lex *lex, char c)
{
	if (*lex->state == INDQTS || *lex->state == INSQTS)
	{
		lex->tail->word[lex->tail->idx++] = c;
		return ;
	}
	if (c == '|')
	{
		handle_pipe(lex);
		return ;
	}
	if (*lex->state == WORD && lex->tail->idx)
	{
		lex->tail->next = gen_token(UNDETERM, 0, lex);
		if (!lex->err)
			lex->tail = lex->tail->next;
	}
	if (c == '<')
		*lex->state = INREDIR;
	else if (c == '>')
		*lex->state = OUTREDIR;
	lex->tail->type = IOTYPE;
	lex->tail->word[lex->tail->idx++] = c;
}

void	handle_pipe(t_lex *lex)
{
	lex->tail->type = PIPE;
	lex->tail->word[lex->tail->idx++] = '|';
	lex->tail->next = gen_token(UNDETERM, 0, lex);
	if (!lex->err)
		lex->tail = lex->tail->next;
	*lex->state = DELIM;
}

/*find handle_expand(t_lex *lex, int dist) in expand.c */
