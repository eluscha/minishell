/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_mid_fts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:19:14 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/30 12:38:20 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*check_word_border(t_lex_state *state, t_tok *tail, char c, int *err)
{
	if (c == ' ' || c == '\t')
	{
		if (*state == DELIM)
			return (tail);
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
	else if (*state >= INREDIR || c == '|')
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
			printf("we are here \n");
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

int	handle_expand(char *start, t_tok *tail, t_data *data, int *err)
{
	char	*varvalue;
	char	**envp;
	int		i;

	envp = data->envp;
	i = 0;
	if (start[i] == '?' && ++i)
		varvalue = ft_itoa(data->st_code);
	else
	{
		while (start[i] && ft_isalnum(start[i]))
			i++;
		if (!i)
		{
			tail->word[tail->idx++] = '$';
			return (0);
		}
		while (*envp)
		{
			if (ft_strncmp(*envp, start, i) == 0 && (*envp)[i] == '=')
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


int	change_word(t_tok *token, char *var, char *start)
{
	int		lenword;
	int		lenvar;
	int		sum;
	char	*newword;

	if (!var)
		return (1);
	lenword = ft_strlen(token->word);
	lenvar = ft_strlen(var);
	sum = lenword + lenvar + ft_strlen(start);
	newword = ft_calloc(sum + 1, sizeof(char));
	if (!newword)
	{
		free(var);
		return (1);
	}
	ft_strlcpy(newword, token->word, lenword + 1);
	ft_strlcat(newword, var, sum + 1);
	free(token->word);
	free(var);
	token->word = newword;
	token->idx += lenvar;
	return (0);
}
