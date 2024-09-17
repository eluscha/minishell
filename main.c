/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:07:50 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/17 11:46:32 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
NON-INTERACTIVE MAIN

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	int		return_value;
	t_tok *head;
	t_tok *tail;

	if (argc > 2)
	{
		printf ("no arguments required, only program name: %s\n", argv[0]);
		return (1);
	}
	init_data(&data, envp);
	tail = lexer(input, NULL, data);
	head = tail->next;
	tail->next = NULL;
	data.cmd = parser(head, &data);
	return_value = execute_loop(&data);
	if (data.cmd)
		clean_exit(NULL, return_value, &data); //
	clean_exit(NULL, 0, &data);
}
*/

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	t_tok	*head;

	if (argc > 1)
	{
		printf ("no arguments required, only program name: %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	init_data(&data, envp);
	while (1)
	{
		head = read_input(&data);
		data.cmd = parser(head, &data);
		if (!data.cmd)
			break ;
		execute_loop(&data);
		free_cmds(data.cmd);
	}
	printf("exit\n");
	exit(clean_exit(NULL, EXIT_SUCCESS, &data));
}

t_tok	*read_input(t_data *data)
{
	char	*input;
	t_tok	*tail;
	t_tok	*head;

	input = readline("minishell> ");
	if (input == NULL) //will happen with ctrl+D
		return (NULL);
	if (*input)
		add_history(input);
	tail = lexer(input, NULL, data);
	while (tail && tail->type >= SQERR && tail->type <= PIPERR)
	{
		free(input);
		input = readline("> ");
		if (input == NULL) //will happen with ctrl+D
			tail = free_tokens(tail->next);
		tail = lexer(input, tail, data);
	}
	if (input)
		free(input);
	if (!tail)
		return (NULL);
	head = tail->next;
	tail->next = NULL;
	return (head);
}
