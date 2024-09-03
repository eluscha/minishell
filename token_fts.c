/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_fts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:50:20 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/03 12:49:39 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_tok	*gen_token(t_toktype type, int len, int *err)
{
	t_tok	*token;

	token = malloc(sizeof(t_tok));
	if (!token)
	{
		*err = 1;
		return (NULL);
	}
	token->next = NULL;
	token->type = type;
	token->idx = 0;
	token->word = ft_calloc(len + 1, sizeof(char));
	if (!token->word)
	{
		free(token);
		*err = 0;
		return (NULL);
	}
	if (type == END)
		ft_strlcpy(token->word, "newline", 8);
	else if (type == NWLINE)
		ft_strlcpy(token->word, "\n", 2);
	return (token);
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
	//printf("idx is %d\n", token->idx);
}

int insert_token(t_tok *token)
{
	char	*word = ft_strdup(token->word + 1);
	int		err = 0;
	t_tok *next = token->next;
	token->next = gen_token(UNDETERM, ft_strlen(word), &err);
	if (err)
		return (1);
	token->next->word = word;
	token->next->next = next;
	token->word[1] = '\0';
	return (0);
}

//free_tokens function goes here

void print_toktype(t_tok *token)
{
	if (token->type == UNDETERM)
		printf("UNDETERM ");
	else if (token->type == END)
		printf("END\n");
	else if (token->type == SQERR)
		printf("SQERR ");
	else if (token->type == DQERR)
		printf("DQERR ");
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
}