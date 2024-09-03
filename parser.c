/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:18:57 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/03 14:18:30 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

//void print_struct(t_cmd *cmd);
void print_toktype(t_tok *token);



t_cmd	*parser(char *input, t_data *data)
{
	int numargs;
	t_tok *tail;
	t_tok *head;
	t_tok *ptr;
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
	head = tail->next;
	numargs = process_tokens(head);
	if (check_syntax(head) == 0)
		cmds = generate_structs(head, numargs);

		/* THIS IS for printing structs
		t_cmd *ptrs = cmds;
		while (ptrs)
		{
			print_struct(ptrs);
			ptrs = ptrs->next;
		}
		*/

	while (head->type != END)
	{

        print_toktype(head);
        printf("%s\n", head->word);
		if (head->word)
			free(head->word);
		ptr = head->next;
		free(head);
		head = ptr;
	}
	free(head->word);
	free(head);

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
	char c;
	t_tok *head;
	int len = ft_strlen(input);
	int i = -1;
	int err = 0;

	if (!tail)
	{
		tail = gen_token(UNDETERM, len, &err); // need to protect malloc
		head = tail;
	}
	else
	{
		head = tail->next;
		tail->next = gen_token(NWLINE, 1, &err);
		if (!err)
		{
			tail = tail->next;
			tail->next = gen_token(UNDETERM, len, &err);
			if (!err)
				tail = tail->next;
		}
	}
	while (input[++i] && !err)
	{
		c = input[i];
		if ((state == WORD || state == DELIM) && (c == ' ' || c == '\t'))
			tail = handle_spaces(&state, tail, len, &err);
		else if (c == '\'' || c == '\"')
			handle_quotes(&state, tail, c);
        else if (c == '|' || c == '>' || c == '<') //handle special chars
        {
			if (state == DELIM)
				state = WORD;
			else if (state == WORD && tail->word[0] && tail->word[0] != c)
            {
				tail->next = gen_token(UNDETERM, len, &err);
				tail = tail->next;
			}
            tail->word[tail->idx++] = c;
        }
		else if (c == '$' && (state == WORD || state == INDQTS))
			i += handle_expand(input + i + 1, tail, data, &err);
		else
		{
			if (state == DELIM)
				state = WORD;
			tail->word[tail->idx++] = c;
			//printf("word is %s\n", tail->word);
		}
		//printf("char is '%c' and state is %i\n", c, state);
	}
	if (state == INSQTS)    //need to add analogous pipe error, seems like
		tail->type = SQERR;
	else if (state == INDQTS)
		tail->type = DQERR;
	else if (state == DELIM)
		tail->type = END;
	else
	{
		tail->next = gen_token(END, 7, &err);
		if (!err)
			tail = tail->next;
	}
	tail->next = head;
	if (err)
	{
		//free_tokens
		return (NULL);
	}
	return (tail);
}


int    io_type(t_tok *token, t_toktype type)
{
	token->type = type;
	char *io_arg;
	int idx;
	if (type == HEREDOC || type == APPEND)
		idx = 2;
	else
		idx = 1;
	if (!token->word[idx])
	{
		token->type = DISCARD;
		if (token->next->type != END)
			token->next->type = type;
	}
	else
	{
		token->type = type;
		io_arg = ft_strdup(token->word + idx);
		if (!io_arg)
			return (1);
		free(token->word);
		token->word = io_arg;
	}
	return (0);
}


int process_tokens(t_tok *token) //maybe return -1 always on malloc err ?
{
	int cmd = 0;
	int args = 0;
	while (token->type != END)
	{
		if (token->word[0] == '|')
		{
			token->type = PIPE;
			cmd = 0;
			if (token->word[1])
				insert_token(token);
		}
		else if (token->type >= HEREDOC)
		{
			token = token->next;
			continue ;
		}
		else if (token->word[0] == '<')
		{
			if (token->word[1] == '<')
				io_type(token, HEREDOC);
			else
				io_type(token, INPUT);
		}
		else if (token->word[0] == '>')
		{
			if (token->word[1] == '>')
				io_type(token, APPEND);
			else
				io_type(token, OUTPUT);
		}
		else if (!cmd)
		{
			token->type = CMD;
			cmd = 1;
		}
		else
		{
			token->type = ARGS;
			args++;
		}
		token = token->next;
	}
	return args;
}

int check_syntax(t_tok *head)
{
	t_toktype ntype;
	int err = 0;
	while(head->type != END)
	{
		ntype = head->next->type;
		if (head->type == PIPE && ntype != CMD && ntype < DISCARD)
			err = 1;
		else if (head->type == DISCARD && ntype < HEREDOC)
			err = 1;
		if (err)
		{
			printf("syntax error near unexpected token `");
			printf("%s\'\n", head->next->word);
			return (1);
		}
		head = head->next;
	}
	return (0);
}

t_cmd *generate_structs(t_tok *head, int numargs)
{
	t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
	cmd->args = ft_calloc(numargs + 2, sizeof(char *));
	t_redirect *ptr;
	t_redirect **ptradr;
	int idx = 1;
	while (head->type != END)
	{
		if (head->type == PIPE)
		{
			cmd->next = generate_structs(head->next, numargs);
			break ;
		}
		else if (head->type == CMD)
			cmd->cmd = ft_strdup(head->word); // need to protect
		else if (head->type == ARGS)
			cmd->args[idx++] = ft_strdup(head->word);
		else if (head->type >= HEREDOC)
		{
			if (head->type >= OUTPUT)
			{
				ptr = cmd->out_redirect;
				ptradr = &cmd->out_redirect;
			}
			else
			{
				ptr = cmd->in_redirect;
				ptradr = &cmd->in_redirect;
			}
			while (ptr)
			{
				ptr = ptr->next;
				ptradr = &ptr->next;
			}
			ptr = ft_calloc(1, sizeof(t_redirect));
			ptr->value = ft_strdup(head->word); //need to protect
			ptr->type = head->type;
			*ptradr = ptr;
		}
		head = head->next;
	}
	return (cmd);
}

/*
void print_struct(t_cmd *cmd)
{
	if (!cmd)
		return ;
	printf("CMD: %s ARGS: ", cmd->cmd);
	int i = -1;
	while (cmd->args[++i])
		printf("%s ", cmd->args[i]);
	printf("%s\n", cmd->args[i]);
	t_redirect *ptr = cmd->in_redirect;
	while (ptr)
	{
		printf("in_redirect type is %d value is %s\n", ptr->type, ptr->value);
		ptr = ptr->next;
	}
	ptr = cmd->out_redirect;
	while (ptr)
	{
		printf("out_redirect type is %d value is %s\n", ptr->type, ptr->value);
		ptr = ptr->next;
	}
}*/
