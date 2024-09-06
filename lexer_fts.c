/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_fts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:19:14 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/06 14:36:10 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*check_word_border(lex_state *state, t_tok *tail, char c, int *err)
{
	if (c == ' ' || c == '\t')
	{
		if (*state == DELIM)
			return (tail);
		*state = DELIM;
		tail->next = gen_token(UNDETERM, 0, err);
		if (!*err)
			tail = tail->next;
	}
	else
		*state = WORD;
	return (tail);
}

void	handle_quotes(lex_state *state, t_tok *tail, char c)
{
	if (c == '\'')
	{
		if (*state == INSQTS)
			*state = WORD;
		else if (*state == WORD)
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

t_tok	*handle_special(lex_state *state, t_tok *tail, char c, int *err)
{
	if (*state == WORD && tail->word[0] && tail->word[0] != c)
	{
		tail->next = gen_token(UNDETERM, 0, err);
		if (!*err)
			tail = tail->next;
	}
	tail->word[tail->idx++] = c;
	return (tail);
}

int	handle_expand(char *start, t_tok *tail, t_data *data, int *err)
{
	char	*varvalue;
	char	**envp;
	int		i;

	envp = data->envp;
	i = 0;
	if (start[i] == '?')
		varvalue = ft_itoa(data->st_code);
	else
	{
		while (start[i] && ft_isalnum(start[i]))
			i++;
		while (*envp)
		{
			if (ft_strncmp(*envp, start, i) == 0 && *envp[i] == '=')
				break ;
			envp++;
		}
		if (!*envp)
			varvalue = (ft_strdup(""));
		else
			varvalue = ft_strdup(*envp + i + 1);
	}
	*err = change_word(tail, varvalue, start + i);
	return (i);
}

t_tok	*set_end(lex_state *state, t_tok *tail, t_tok *head, int *err)
{
	if (*state == INSQTS)
		tail->type = SQERR;
	else if (*state == INDQTS)
		tail->type = DQERR;
	else if (tail->word[tail->idx - 1] == '|')
		tail->type = PIPERR;
	else if (*state == DELIM)
	{
		tail->type = END;
		ft_strlcpy(tail->word, "newline", 8);
	}
	else
	{
		tail->next = gen_token(END, 7, err);
		if (!*err)
			tail = tail->next;
	}
	if (*err)
		tail = free_tokens(head);
	else
		tail->next = head;
	return (tail);
}
