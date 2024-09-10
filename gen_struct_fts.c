/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_struct_fts.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:59:34 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/09 10:46:23 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_struct(int numargs, int *err)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
	{
		*err = -1; 
		return (NULL);
	}
	cmd->args = ft_calloc(numargs + 2, sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		*err = -1;
		return (NULL);
	}
	return (cmd);
}

int	fill_struct(t_tok *head, t_cmd *cmd, int *idx)
{
	if (head->type == CMD)
	{
		cmd->cmd = ft_strdup(head->word);
		if (!cmd->cmd)
			return (-1);
	}
	else if (head->type == ARGS)
	{
		cmd->args[*idx] = ft_strdup(head->word);
		if (!cmd->args[*idx])
			return (-1);
		*idx = *idx + 1;
	}
	else if (head->type >= HEREDOC)
		return (fill_redirect(head, cmd));
	return (0);
}

int fill_redirect(t_tok *head, t_cmd *cmd)
{
	t_redirect **ptradr;

	if (head->type >= OUTPUT)
		ptradr = &cmd->out_redirect;
	else
		ptradr = &cmd->in_redirect;
	while (*ptradr)
		ptradr = &(*ptradr)->next;
	*ptradr = ft_calloc(1, sizeof(t_redirect));
	if (!*ptradr)
		return (-1);
	(*ptradr)->value = ft_strdup(head->word); 
	if (!(*ptradr)->value)
		return (-1);
	(*ptradr)->type = head->type;
	return (0);
}


t_cmd	*free_cmd(t_cmd *cmd)
{
	t_redirect	*ptr;
	int			i;

	ptr = cmd->in_redirect;
	while (ptr)
		free_redirect(ptr);
	ptr = cmd->out_redirect;
	while (ptr)
		free_redirect(ptr);
	i = 0;
	while(cmd->args[++i])
		free(cmd->args[i]);
	//if (cmd->args[0])
	//	free(cmd->args[0]);
	free(cmd->args);
	free(cmd);
	return (NULL);
}

void	free_redirect(t_redirect *ptr)
{
	t_redirect	*tofree;

	if (ptr->value)
		free(ptr->value);
	tofree = ptr;
	ptr = ptr->next;
	free(tofree);
}
