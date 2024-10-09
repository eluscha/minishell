/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_edge_fts.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:40:04 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/09 14:27:01 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lex *init_lex(char *input, t_tok *tail, t_data *data)
{
	static t_lex_state	state;
	t_lex	*lex;

	lex = ft_calloc(1, sizeof(t_lex));
	if (!lex)
		return (NULL);
	lex->envp = data->envp;
	lex->st_code = data->st_code;
	lex->err = 0;
	lex->tail = tail;
	lex->state = &state;
	set_start(lex, ft_strlen(input));
	return (lex);
}

void	set_start(t_lex *lex, int len)
{
	if (!lex->tail)
	{
		lex->tail = gen_token(UNDETERM, len, lex);
		lex->head = lex->tail;
	}
	else
	{
		lex->head = lex->tail->next;
		if (lex->tail->type == PIPERR)
		{
			lex->tail->next = gen_token(UNDETERM, len, lex);
			if (!lex->err)
				lex->tail = lex->tail->next;
		}
		else
			extend_word(lex->tail, len, lex);
	}
}

t_tok	*gen_token(t_toktype type, int len, t_lex *lex)
{
	t_tok		*token;
	static int	input_len;

	if (!len)
		len = input_len;
	else
		input_len = len;
	token = malloc(sizeof(t_tok));
	if (!token)
	{
		lex->err = -1;
		return (NULL);
	}
	token->next = NULL;
	token->type = type;
	token->idx = 0;
	token->word = ft_calloc(len + 1, sizeof(char));
	if (!token->word)
	{
		free(token);
		lex->err = -1;
		return (NULL);
	}
	return (token);
}

void	extend_word(t_tok *tail, int len, int *err)
{
	int		oldlen;
	char	*newword;

	oldlen = ft_strlen(tail->word);
	newword = ft_calloc(oldlen + len + 2, sizeof(char));
	if (!newword)
	{
		*err = -1;
		return ;
	}
	ft_strlcpy(newword, tail->word, oldlen + 1);
	free(tail->word);
	tail->word = newword;
	tail->word[tail->idx++] = '\n';
}

t_tok	*set_end(t_lex_state *state, t_tok *tail, char c, int *err)
{
	if (*state == INSQTS)
		tail->type = SQERR;
	else if (*state == INDQTS)
		tail->type = DQERR;
	else if (c == '|')
		tail->type = PIPERR;
	else if (*state == DELIM || *state == EXPAND)
	{
		tail->type = END;
		free(tail->word);
		tail->word = ft_strdup("newline");
		if (!tail->word)
			*err = -1;
	}
	else
	{
		*state = DELIM;
		tail->next = gen_token(END, 7, err);
		if (!*err)
			tail = tail->next;
	}
	return (tail);
}

t_tok *connect_ends(t_tok* tail, t_tok *head, int *err)
{
	if (*err)
		return (free_tokens(head));
	tail->next = head;
	return (tail);
}

/*won't need this function in a final version*/
void	print_toktype(t_tok *token)
{
	if (token->type == UNDETERM)
		printf("UNDETERM ");
	else if (token->type == END)
		printf("END\n");
	else if (token->type == SQERR)
		printf("SQERR ");
	else if (token->type == DQERR)
		printf("DQERR ");
	else if (token->type == PIPERR)
		printf("PIPERR ");
	else if (token->type == PIPE)
		printf("PIPE ");
	else if (token->type == CMD)
		printf("CMD ");
	else if (token->type == ARGS)
		printf("ARGS ");
	else if (token->type == IOTYPE)
		printf("IOTYPE ");
	else if (token->type == HEREDOC)
		printf("HEREDOC ");
	else if (token->type == INPUT)
		printf("INPUT ");
	else if (token->type == OUTPUT)
		printf("OUTPUT ");
	else if (token->type == APPEND)
		printf("APPEND ");
}
