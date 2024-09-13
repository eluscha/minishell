/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:18:57 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/13 12:30:05 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

//void print_struct(t_cmd *cmd);

t_cmd	*parser(char *input, t_data *data)
{
	int numargs;
	int numredir;
	t_tok *tail;
	t_tok *head;
	t_cmd *cmds = NULL;
	/*  THIS is preliminary readline for multiple line input..
	while (1)
	{
		if (input)
			free(input);
		input = readline("prompt> ");
		if (input == NULL) //will happen with ctrl+D
			break;
		if (*input)
			add_history(input);
		tail = lexer(input, DELIM, NULL, data);
		while (tail->type == SQERR)
		{
			input = readline("> ");
			if (input == NULL) //will happen with ctrl+D
				break;
			tail = lexer(input, INSQTS, NULL, data);
		}
		while (tail->type == DQERR)
		{
			input = readline("> ");
			if (input == NULL) //will happen with ctrl+D
				break;
			tail = lexer(input, INDQTS, tail, data);
		}
		head = tail->next;
		*/

	tail = lexer(input, DELIM, NULL, data);
	if (!tail)
		return (NULL);
	head = tail->next;
	tail->next = NULL;
	if (process_tokens(head, &numargs, &numredir) == -1)
	{
		//printf("process tokens returned -1\n");
		free_tokens(head);
		return (NULL);
	}
	tail = check_syntax(head);
	if (tail->type != END || get_heredoc(head, tail, data) == -1)
	{
		//printf("check syntax returned early or heredoc has error\n");
		cmds = NULL;
	}
	else
		cmds = generate_structs(head, numargs, numredir);
	//THIS IS for printing structs
	/*t_cmd *ptrs = cmds;
	while (ptrs)
	{
		print_struct(ptrs);
		ptrs = ptrs->next;
	}
	*/
	//THIS IS for printing tokens
	/*
	t_tok *ptr = head;
	while (ptr->type != END)
	{
        print_toktype(ptr);
        printf("%s\n", ptr->word);
		ptr = ptr->next;
	}
	*/
	free_tokens(head);
	return (cmds);
	}


/*
char    *expand(char *start, int *lenvar, char **envp)
{
	int i = 0;
	while(start[i] && ft_isalnum(start[i]))
		i++;
	*lenvar = i;
	char *var = ft_strdup(start);
	var[i] = '\0';
	int len = ft_strlen(var);
	while (*envp)
	{
		if (ft_strncmp((const char *) *envp, var, len) == 0 || *envp[len] == '=')
			break ;
		envp++;
	}
	free(var);
	if (!*envp)
		return (ft_strdup(""));
	return (ft_strdup(*envp + len + 1)); //need to free though later
}
*/

t_tok *lexer(char *input, lex_state state, t_tok *tail, t_data *data)
{
	char	c;
	t_tok	*head;
	int		i;
	int		err;

	err = 0;
	tail = set_start(tail, &head, ft_strlen(input), &err);
	i = -1;
	while (input[++i] && !err)
	{
		c = input[i];
		if (state == DELIM || state == WORD)
			tail = check_word_border(&state, tail, c, &err);
		if (c == '\'' || c == '\"')
			handle_quotes(&state, tail, c);
		else if (state == WORD && (c == '|' || c == '>' || c == '<'))
			tail = handle_special(&state, tail, c, &err);
		else if (c == '$' && (state == WORD || state == INDQTS))
			i += handle_expand(input + i + 1, tail, data, &err);
		else if (state != DELIM)
			tail->word[tail->idx++] = c;
		//printf("state is %d c is %c word is %s\n", state, c, tail->word);
	}
	tail = set_end(&state, tail, head, &err);
	return (tail);
}

t_tok	*check_syntax(t_tok *head)
{
	t_toktype	ntype;
	int			err;

	err = 0;
	while (head->type != END)
	{
		ntype = head->next->type;
		if (head->type == PIPE && ntype != CMD && ntype < DISCARD)
			err = 1;
		else if (head->type == DISCARD && ntype < HEREDOC)
			err = 1;
		head = head->next;
		if (err)
		{
			printf("syntax error near unexpected token `");
			printf("%s\'\n", head->word);
			break ;
		}
	}
	return (head);
}
