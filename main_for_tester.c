#include "minishell.h"

int lastsignal;

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	t_tok *head;
	t_tok *tail;

	if (argc != 2)
	{
		printf ("one argument requred: cmd as a string\n");
		return (1);
	}
	init_data(&data, envp);
    init_signals(&data);
	tail = lexer(argv[1], NULL, &data);
	head = tail->next;
	tail->next = NULL;
	data.cmd = parser(head, &data);
	if (data.cmd)
		execute_loop(&data);
	exit(clean_exit(NULL, EXIT_SUCCESS, &data));
}
