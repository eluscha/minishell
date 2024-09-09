/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:56:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/09 10:45:14 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>

typedef enum e_lex_state
{
	WORD,
	DELIM,
	INSQTS,
	INDQTS
}	lex_state;

typedef enum e_toktype
{
	UNDETERM,
	END,
	SQERR,
	DQERR,
	PIPERR,
	NWLINE,
	PIPE,
	CMD,
	ARGS,
	DISCARD,
	HEREDOC,
	INPUT,
	OUTPUT,
	APPEND
}	t_toktype;

typedef struct s_tok
{
	char *word;
	int idx;
	struct s_tok *next;
	t_toktype type;
}	t_tok;

typedef struct cmd
{
	char			*cmd;
	char			**args;
	struct redirect	*in_redirect;
	struct redirect	*out_redirect;
	struct cmd		*next;
	int				builtin;
}	t_cmd;

typedef struct redirect
{
	t_toktype		type;
	char			*value;
	struct redirect	*next;
}	t_redirect;

typedef struct pids
{
	int			pid;
	struct pids	*next;
}	t_pids;

typedef struct data
{
	t_cmd	*cmd;
	t_pids	*pids;
	int		st_code;
	int		fd[2];
	char	**envp;
	char	**paths;
	char	*tty_in;
	char	*tty_out;
}	t_data;

int		clean_exit(char *msg, int r_value, t_data *data);
t_cmd	*parce(void);
int		redirect(t_cmd *cmd);
int		new_pid(int pid, t_data *data);
int		get_path(t_cmd *cmd, t_data *data);
int		check_builtin(t_cmd *cmd);
// void	read_input(t_data *data);
int		execute_loop(t_data *data);
void	init_data(t_data *data, char **envp);

/* parser.c */
t_cmd	*parser(char *input, t_data *data);
t_tok	*lexer(char *input, lex_state state, t_tok *tail, t_data *data);
int		process_tokens(t_tok *token);
int		check_syntax(t_tok *head);
t_cmd 	*generate_structs(t_tok *head, int numargs);

/* lexer_mid_fts.c */
t_tok	*check_word_border(lex_state *state, t_tok *tail, char c, int *err);
void	handle_quotes(lex_state *state, t_tok *tail, char c);
t_tok	*handle_special(lex_state *state, t_tok *tail, char c, int *err);
int		handle_expand(char *start, t_tok *tail, t_data *data, int *err);
int		change_word(t_tok *token, char *var, char *start);

/* lexer_edge_fts.c */
t_tok	*set_start(t_tok *tail, t_tok **head, int len, int *err);
t_tok	*gen_token(t_toktype type, int len, int *err);
t_tok	*set_end(lex_state *state, t_tok *tail, t_tok *head, int *err);
t_tok	*free_tokens(t_tok *head);
void	print_toktype(t_tok *token);

/* process_tok_fts.c */
int		handle_pipe(t_tok *token, int *cmd);
int		handle_notpipe(t_tok *token, int cmd);
int		io_type(t_tok *token, t_toktype type);

/* gen_struct_fts.c */
t_cmd	*init_struct(int numargs, int *err);
int		fill_struct(t_tok *head, t_cmd *cmd, int *idx);
int		fill_redirect(t_tok *head, t_cmd *cmd);
t_cmd	*free_cmd(t_cmd *cmd);
void	free_redirect(t_redirect *ptr);

#endif
