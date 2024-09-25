/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_edge_fts.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:40:04 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/25 11:59:40 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*set_start(t_tok *tail, t_tok **head, int len, int *err)
{
	if (!tail)
	{
		tail = gen_token(UNDETERM, len, err);
		*head = tail;
	}
	else
	{
		*head = tail->next;
		if (tail->type == PIPERR)
		{
			tail->next = gen_token(NWLINE, 1, err);
			if (!err)
				tail = tail->next;
		}
		else
			extend_word(tail, len, err);
	}
	return (tail);
}

t_tok	*gen_token(t_toktype type, int len, int *err)
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
		*err = -1;
		return (NULL);
	}
	token->next = NULL;
	token->type = type;
	token->idx = 0;
	token->word = ft_calloc(len + 1, sizeof(char));
	if (!token->word)
	{
		free(token);
		*err = -1;
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
	else if (*state == DELIM)
	{
		tail->type = END;
		ft_strlcpy(tail->word, "newline", 8);
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


t_tok	*free_tokens(t_tok *head)
{
	t_tok	*ptr;

	while (head != NULL)
	{
		if (head->word)
			free(head->word);
		ptr = head->next;
		free(head);
		head = ptr;
	}
	return (NULL);
}

/*won't need this function in a final version*/
void	print_toktype(t_tok *token)
{
	if (token->type == UNDETERM)
		printf("UNDETERM ");
	if (token->type == NOSPECIAl)
		printf("NOSPECIAl ");
	else if (token->type == END)
		printf("END\n");
	else if (token->type == SQERR)
		printf("SQERR ");
	else if (token->type == DQERR)
		printf("DQERR ");
	else if (token->type == PIPERR)
		printf("PIPERR ");
	else if (token->type == NWLINE)
		printf("NWLINE ");
	else if (token->type == PIPE)
		printf("PIPE ");
	else if (token->type == CMD)
		printf("CMD ");
	else if (token->type == ARGS)
		printf("ARGS ");
	else if (token->type == HEREDOC)
		printf("HEREDOC ");
	else if (token->type == INPUT)
		printf("INPUT ");
	else if (token->type == OUTPUT)
		printf("OUTPUT ");
	else if (token->type == APPEND)
		printf("APPEND ");
	else if (token->type == DISCARD)
		printf("DISCARD ");
	else if (token->type == HDDASH)
		printf("HDDASH ");
}
