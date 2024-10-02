/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:25:11 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/30 13:17:17 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_tokens(t_tok *token, int *numargs, int *numredir)
{
	int	cmd;

	cmd = 0;
	*numargs = 0;
	*numredir = 0;
	while (token->type != END)
	{
		if (token->type == PIPE)
			cmd = 0;
		else if (token->type == IOTYPE)
			handle_iotype(token, numredir);
		else if (token->type == UNDETERM)
		{
			if (!cmd++)
				token->type = CMD;
			else 
			{
				token->type = ARGS;
				*numargs = *numargs + 1;
			}
		}
		token = token->next;
	}
}

void	handle_iotype(t_tok *token, int *numredir)
{
	if (token->next->type == END)
		return ;
	*numredir = *numredir + 1;
	if (token->word[0] == '<')
	{
		if (!token->word[1])
			token->next->type = INPUT;
		else
			token->next->type = HEREDOC;
	}
	else if (token->word[0] == '>')
	{
		if (!token->word[1])
			token->next->type = OUTPUT;
		else
			token->next->type = APPEND;
	}
}
