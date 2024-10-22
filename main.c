/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:07:50 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/16 12:12:21 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_lastsignal;

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	t_tok	*head;

	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd ("no arguments required, only program name\n", 2);
		exit(EXIT_FAILURE);
	}
	init_data(&data, envp);
	main_loop(&data, &head);
	printf("exit\n");
	close (data.std_in);
	exit(clean_exit(NULL, data.st_code, &data));
}

void	main_loop(t_data *data, t_tok **head)
{
	while (1)
	{
		sigaction(SIGINT, data->sa, NULL);
		signal(SIGQUIT, SIG_IGN);
		if (g_lastsignal == SIGINT)
			printf("\n");
		if (g_lastsignal == SIGQUIT)
			write(2, "Quit\n", 5);
		g_lastsignal = 0;
		*head = read_input(data);
		if (!(*head))
			break ;
		if ((*head)->type == END)
		{
			g_lastsignal = 0;
			free_tokens(*head);
			continue ;
		}
		data->cmd = parser(*head, data);
		if (!data->cmd)
			continue ;
		signal(SIGINT, SIG_IGN);
		execute_loop(data);
		data->cmd = free_cmds(data->cmd);
	}
}

t_tok	*read_input(t_data *data)
{
	char	*input;
	t_tok	*tail;
	t_tok	*head;

	input = readline("minishell> ");
	if (g_lastsignal)
		data->st_code = g_lastsignal + 128;
	if (!input)
		return (NULL);
	if (*input)
		add_history(input);
	tail = lexer(input, NULL, data, 1);
	if (tail && tail->type >= SQERR && tail->type <= PIPERR)
		tail = accept_multiline_input(tail, &input, data);
	if (input)
		free(input);
	if (!tail)
		return (NULL);
	head = tail->next;
	tail->next = NULL;
	return (head);
}

void	handle_mltiline_sigint(int sig)
{
	g_lastsignal = sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_putstr_fd("\nminishell", 1);
	rl_redisplay();
}

t_tok	*accept_multiline_input(t_tok *tail, char **input, t_data *data)
{
	char qt;

	while (tail && tail->type >= SQERR && tail->type <= PIPERR)
	{
		g_lastsignal = 0;
		signal(SIGINT, &handle_mltiline_sigint);
		qt = '\0';
		if (tail->type == SQERR)
			qt = '\'';
		else if (tail->type == DQERR)
			qt = '\"';
		free(*input);
		*input = readline("> ");
		if (!*input)
		{
			data->st_code = 2;
			if (qt)
				printf("unexpected EOF while looking for matching `%c\'\n", qt);
			printf("syntax error: unexpected end of file\n");
			free_tokens(tail->next);
			tail = ft_calloc(1, sizeof(t_tok));
			if (!tail)
				return (NULL);
			tail->word = ft_strdup("end of file");
			if (!tail->word)
			{
				free(tail);
				return (NULL);
			}
			tail->type = END;
			tail->next = tail;
			return (tail);
		}
		if (g_lastsignal)
		{
			if (**input)
				add_history(*input);
			data->st_code = g_lastsignal + 128;
			free_tokens(tail->next);
			tail = lexer(*input, NULL, data, 1);
			if (tail && tail->type >= SQERR && tail->type <= PIPERR)
				tail = accept_multiline_input(tail, input, data);
			return (tail);
		}
		tail = lexer(*input, tail, data, 0);
	}
	return (tail);
}
