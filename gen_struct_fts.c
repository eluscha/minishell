/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_struct_fts.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:59:34 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/10 14:11:18 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_struct(int numargs, int numredir, int *err)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (cmd)
	{
		cmd->args = ft_calloc(numargs + 2, sizeof(char *));
		if (cmd->args)
		{
			cmd->redirect = ft_calloc(numredir + 1, sizeof(t_redirect));
			if (cmd->redirect)
				return (cmd);
		}
	}
	free_cmd(cmd);
	*err = -1;
	return (NULL);
}

int	fill_struct(t_tok *head, t_cmd *cmd, int *idx_a, int *idx_r)
{
	if (head->type == CMD)
	{
		cmd->cmd = ft_strdup(head->word);
		if (!cmd->cmd)
			return (-1);
	}
	else if (head->type == ARGS)
	{
		cmd->args[*idx_a] = ft_strdup(head->word);
		if (!cmd->args[*idx_a])
			return (-1);
		*idx_a = *idx_a + 1;
	}
	else if (head->type >= HEREDOC)
	{
		cmd->redirect[*idx_r].type = head->type;
		cmd->redirect[*idx_r].value = ft_strdup(head->word);
		if (!cmd->redirect[*idx_r].value)
			return (-1);
		*idx_r = *idx_r + 1;
	}
	return (0);
}

t_cmd	*free_cmd(t_cmd *cmd)
{
	int			i;

	if (!cmd)
		return (NULL);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[++i])
			free(cmd->args[i]);
		if (cmd->args[0])
			free(cmd->args[0]);
		free(cmd->args);
	}
	if (cmd->redirect)
	{
		i = -1;
		while (cmd->redirect[++i].value)
			free(cmd->redirect[i].value);
		free(cmd->redirect);
	}
	free(cmd);
	return (NULL);
}
