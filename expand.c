/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:27:21 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/14 13:05:49 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_expand(t_lex *lex, int dist)
{
	char	*varvalue;
	int		i;
	char	*start;

	i = 0;
	if (*lex->state != WORD && *lex->state != INDQTS)
		lex->tail->word[lex->tail->idx++] = '$';
	else
	{
		start = lex->input + dist;
		if (start[i] == '?' && ++i)
			varvalue = ft_itoa(lex->data->st_code);
		else
		{
			while (start[i] && ft_isalnum(start[i]))
				i++;
			if (!i)
				lex->tail->word[lex->tail->idx++] = '$';
			else
				varvalue = find_var(lex->data->envp, start, i);
		}
		if (i)
			lex->err = change_word(lex->tail, varvalue, start + i, lex->state);
	}
	return (i);
}

char	*find_var(char **list, char *start, int i)
{
	while (*list)
	{
		if (ft_strncmp(*list, start, i) == 0 && (*list)[i] == '=')
			break ;
		list++;
	}
	if (!*list)
		return (ft_strdup(""));
	else
		return (ft_strdup(*list + i + 1));
}

int	change_word(t_tok *token, char *var, char *start, t_lex_state *state)
{
	int		lenword;
	int		lenvar;
	int		sum;
	char	*newword;

	if (!var)
		return (-1);
	lenword = ft_strlen(token->word);
	lenvar = ft_strlen(var);
	sum = lenword + lenvar + ft_strlen(start);
	newword = ft_calloc(sum + 1, sizeof(char));
	if (!newword)
	{
		free(var);
		return (-1);
	}
	ft_strlcpy(newword, token->word, lenword + 1);
	ft_strlcat(newword, var, sum + 1);
	free(token->word);
	free(var);
	token->word = newword;
	token->idx += lenvar;
	if (!token->idx && state)
		*state = EXPAND;
	return (0);
}
