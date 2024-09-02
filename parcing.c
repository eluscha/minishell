/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:55:41 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/02 09:50:42 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_input(t_data *data)
{
	char	*input;

	while (1)
	{
		input = readline("minishell:");
		if (!input)
			break ;
		if (*input)
		{
			data->cmd = parce();
			execute_loop(data);
		}
		free (input);
	}
}

t_redirect	*infiles(void)
{
	t_redirect	*infile;
	t_redirect	*infile2;

	infile = malloc(sizeof(t_redirect));
	infile->type = 1;
	infile->value = "infile";
	infile2 = malloc(sizeof(t_redirect));
	infile2->type = 1;
	infile2->value = "infile1";
	infile2->next = NULL;
	infile->next = infile2;
	return (infile);
}

t_redirect	*outfiles(void)
{
	t_redirect	*outfile;
	t_redirect	*outfile2;

	outfile = malloc(sizeof(t_redirect));
	outfile->type = 1;
	outfile->value = "outfile";
	outfile2 = malloc(sizeof(t_redirect));
	outfile2->type = 2;
	outfile2->value = "infile";
	outfile2->next = NULL;
	outfile->next = outfile2;
	return (outfile);
}

t_cmd	*parce(void)
{
	t_cmd	*cmd;
	t_cmd	*cmd2;

	cmd = malloc(sizeof(t_cmd));
	cmd2 = malloc(sizeof(t_cmd));
	cmd->cmd = ft_strdup("cat");
	cmd->args = malloc(sizeof(char *) * 3);
	cmd -> args[0] = NULL;
	cmd -> args[1] = NULL;
	cmd -> args[2] = NULL;
	cmd->in_redirect = infiles();
	cmd->out_redirect = NULL;
	cmd->next = cmd2;
	cmd2->args = malloc(sizeof(char *) * 4);
	cmd2 -> args[0] = NULL;
	cmd2 -> args[1] = "-l";
	cmd2 -> args [2] = NULL;
	cmd2 -> args[3] = NULL;
	cmd2->cmd = ft_strdup("wc");
	cmd2->in_redirect = NULL;
	cmd2->out_redirect = NULL;
	cmd2->next = NULL;
	return (cmd);
}
