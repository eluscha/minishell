/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:56:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 14:17:36 by eusatiko         ###   ########.fr       */
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


t_cmd	*parser(char *input, t_data *data);
t_tok *lexer(char *input, lex_state state, t_tok *tail, t_data *data);
/* lexer_fts.c */
t_tok	*handle_spaces(lex_state *state, t_tok *tail, int len, int *err);
void	handle_quotes(lex_state *state, t_tok *tail, char c);
int	handle_expand(char *start, t_tok *tail, t_data *data, int *err);
/* token_fts.c */
t_tok *gen_token(t_toktype type, int len, int *err);
int	change_word(t_tok *token, char *var, char *start);
int insert_token(t_tok *token);
void print_toktype(t_tok *token);
/* process_tokens */
int process_tokens(t_tok *head);
int    io_type(t_tok *token, t_toktype type);
int check_syntax(t_tok *head);
t_cmd *generate_structs(t_tok *head, int numargs);

#endif
