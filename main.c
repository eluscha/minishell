/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:07:50 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/01 10:31:46 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int lastsignal;

void	main_loop(t_data *data, t_tok **head)
{
	while (1)
	{
		sigaction(SIGQUIT, data->sa_quit, NULL);
		sigaction(SIGINT, data->sa, NULL);
		lastsignal = 0;
		*head = read_input(data);
		if (!*head) //ctrl + D
		{
			data->cmd = NULL;
			break ;
		}
		if ((*head)->type == END)
		{
			free_tokens(*head);
			continue ;
		}
		else
			data->cmd = parser(*head, data);
		if (!data->cmd)
			continue ;
		sigaction(SIGINT, data->sa_ex, NULL);
		sigaction(SIGQUIT, data->sa_quit_ex, NULL);
		execute_loop(data);
		free_cmds(data->cmd);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	t_tok	*head;

	(void)argv;
	//if (!isatty(0) || !isatty(1))
	//	return (clean_exit("piping ./execs is not supported\n", errno, NULL));
	init_signals(&data);
	// sigaction(SIGQUIT, data.sa_quit, NULL);
	// sigaction(SIGINT, data.sa, NULL);
	if (argc > 1)
	{
		ft_putstr_fd ("no arguments required, only program name\n", 2);
		exit(EXIT_FAILURE);
	}
	init_data(&data, envp);
	main_loop(&data, &head);
	printf("exit\n");
	exit(clean_exit(NULL, EXIT_SUCCESS, &data)); //when should main return sth other than 0?
}

t_tok	*read_input(t_data *data)
{
	char	*input;
	t_tok	*tail;
	t_tok	*head;

	input = readline("minishell> "); //
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
