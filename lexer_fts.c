/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_fts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:19:14 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/03 14:18:04 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tok	*handle_spaces(lex_state *state, t_tok *tail, int len, int *err)
{
	if (*state == DELIM)
		return (tail);
	*state = DELIM;
	tail->next = gen_token(UNDETERM, len, err);
	if (!*err)
		tail = tail->next;
	return (tail);
}

void	handle_quotes(lex_state *state, t_tok *tail, char c)
{
	if (c == '\'')
	{
		if (*state == INSQTS)
			*state = WORD;
		else if (*state == DELIM || *state == WORD)
			*state = INSQTS;
		else
			tail->word[tail->idx++] = c;
	}
	else if (c == '\"') //handle double quote
	{
		if (*state == INDQTS)
			*state = WORD;
		else if (*state == DELIM || *state == WORD)
			*state = INDQTS;
		else
			tail->word[tail->idx++] = c;
	}
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




