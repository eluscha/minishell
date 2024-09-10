/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:18:57 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/10 11:43:38 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

//void print_struct(t_cmd *cmd);

t_cmd	*parser(char *input, t_data *data)
{
	int numargs;
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
	numargs = process_tokens(head);
	if (numargs == -1)
	{
		free_tokens(head);
		return (NULL);
	}
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
			tail = handle_special(tail, c, &err);
		else if (c == '$' && (state == WORD || state == INDQTS))
			i += handle_expand(input + i + 1, tail, data, &err);
		else if (state != DELIM)
			tail->word[tail->idx++] = c;
		//printf("i is %i, state is %i, word is %s\n", i, state, tail->word);
	}
	tail = set_end(&state, tail, head, &err);
	return (tail);
}

int	process_tokens(t_tok *token)
{
	int	cmd;
	int	args;
	int	err;

	cmd = 0;
	args = 0;
	err = 0;
	while (token->type != END)
	{
		if (token->word[0] == '|')
		{
			token->type = PIPE;
			cmd = 0;
		}
		else
			err = handle_notpipe(token, cmd);
		if (token->type == CMD)
			cmd = 1;
		else if (token->type == ARGS)
			args++;
		token = token->next;
	}
	if (err)
		return (err);
	return (args);
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
	t_cmd *cmd;
	int idx;
	int	err;

	err = 0;
	cmd = init_struct(numargs, &err);
	idx = 1;
	while (head->type != END && !err)
	{
		if (head->type == PIPE)
		{
			cmd->next = generate_structs(head->next, numargs);
			if (!cmd->next)
				cmd = free_cmd(cmd);
			break ;
		}
		err = fill_struct(head, cmd, &idx);
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
