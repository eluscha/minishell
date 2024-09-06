/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_fts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:50:20 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/06 10:51:17 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}
