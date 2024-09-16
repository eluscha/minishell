/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 10:18:57 by eusatiko          #+#    #+#             */
/*   Updated: 2024/09/16 11:23:01 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

//void print_struct(t_cmd *cmd);

t_cmd	*parser(t_tok *head, t_data *data)
{
	int numargs;
	int numredir;
	t_tok *tail;
	t_cmd *cmds = NULL;
		
	if (process_tokens(head, &numargs, &numredir) == -1)
	{
		//printf("process tokens returned -1\n");
		free_tokens(head);
		return (NULL);
	}
	tail = check_syntax(head);
	if (tail->type != END || get_heredoc(head, tail, data) == -1)
		cmds = NULL;
	else
		cmds = generate_structs(head, numargs, numredir);
	//THIS IS for printing structs
	/*
	t_cmd *ptrs = cmds;
	while (ptrs)
	{
		print_struct(ptrs);
		ptrs = ptrs->next;
	}
	*/
	//THIS IS for printing tokens
	
	t_tok *ptr = head;
	while (ptr->type != END)
	{
        print_toktype(ptr);
        printf("%s\n", ptr->word);
		ptr = ptr->next;
	}
	
	free_tokens(head);
	return (cmds);
	}

t_tok *lexer(char *input, t_tok *tail, t_data *data)
{
	char	c;
	t_tok	*head;
	int		i;
	int		err;
	static lex_state state;

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
	}
	tail = set_end(&state, tail, c, &err);
	if (err)
		tail = free_tokens(head);
	else
		tail->next = head;
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
