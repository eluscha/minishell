/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mid_fts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:19:14 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/08 13:27:42 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*check_word_border(t_lex_state *state, t_tok *tail, char c, int *err)
{
	if (c == ' ')
	{
		if (*state == DELIM || *state == EXPAND)
		{
			*state = DELIM;
			return (tail);
		}
		*state = DELIM;
		tail->next = gen_token(UNDETERM, 0, err);
		if (!*err)
			tail = tail->next;
		return (tail);
	}
	else if (*state == INREDIR && tail->idx == 1 && c == '<')
		return (tail);
	else if (*state == OUTREDIR && tail->idx == 1 && c == '>')
		return (tail);
	else if (*state >= INREDIR || (c == '|' && tail->idx))
	{
		tail->next = gen_token(UNDETERM, 0, err);
		if (!*err)
			tail = tail->next;
	}
	*state = WORD;
	return (tail);
}

void	handle_quotes(t_lex_state *state, t_tok *tail, char c)
{
	if (c == '\'')
	{
		if (*state == INSQTS)
			*state = WORD;
		else if (*state == WORD )
			*state = INSQTS;
		else
			tail->word[tail->idx++] = c;
	}
	else if (c == '\"')
	{
		if (*state == INDQTS)
			*state = WORD;
		else if (*state == WORD)
			*state = INDQTS;
		else
			tail->word[tail->idx++] = c;
	}
}

t_tok	*handle_special(t_lex_state *state, t_tok *tail, char c, int *err)
{
	if (*state != INDQTS && *state != INSQTS)
	{
		if (c == '|')
		{
			tail->type = PIPE;
			tail->word[tail->idx++] = c;
			tail->next = gen_token(UNDETERM, 0, err);
			if (!*err)
				tail = tail->next;
			*state = DELIM;
			return (tail);
		}
		else if (*state == WORD && tail->idx)
		{
			tail->next = gen_token(UNDETERM, 0, err);
			if (!*err)
				tail = tail->next;
		}
		if (c == '<')
			*state = INREDIR;
		else if (c == '>')
			*state = OUTREDIR;
		tail->type = IOTYPE;
	}
	tail->word[tail->idx++] = c;
	return (tail);
}

