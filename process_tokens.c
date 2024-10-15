/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:25:11 by eusatiko          #+#    #+#             */
/*   Updated: 2024/10/15 09:07:41 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_tokens(t_tok *token, int *numargs, int *numredir)
{
	int	cmd;

	cmd = 0;
	while (token->type != END)
	{
		if (token->type == PIPE)
			cmd = 0;
		else if (token->type == IOTYPE)
			handle_iotype(token, numredir);
		else if (token->type <= PIPERR)
		{
			if (!cmd)
			{
				cmd = 1;
				token->type = CMD;
			}
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
	if (token->next->type != UNDETERM)
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
