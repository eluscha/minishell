/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:56:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/10 17:17:02 by auspensk         ###   ########.fr       */
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
# include <dirent.h>

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
	char			*word;
	int				idx;
	struct s_tok	*next;
	t_toktype		type;
}	t_tok;

typedef struct cmd
{
	char			*cmd;
	char			**args;
	struct redirect	*redirect;
	struct cmd		*next;
	int				builtin;
}	t_cmd;

typedef struct redirect
{
	t_toktype		type;
	char			*value;
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

typedef enum cmd_type
{
	UNSET,
	EXPORT
}	t_cmd_type;

typedef struct export
{
	char			*arg;
	char			*key;
	t_cmd_type		type;
}	t_export;

int		clean_exit(char *msg, int r_value, t_data *data);
int		redirect(t_cmd *cmd);
int		new_pid(int pid, t_data *data);
int		check_command(t_cmd *cmd, t_data *data);
int		check_builtin(t_cmd *cmd, t_data *data);
int		ft_echo(t_cmd *cmd, t_data *data);
// void	read_input(t_data *data);
int		execute_loop(t_data *data);
void	init_data(t_data *data, char **envp);
int		ft_export(t_cmd *cmd, t_data *data);
int		print_array(char **array);
int		ft_unset(t_cmd *cmd, t_data *data);
int		find_key(t_export *export, char **envp);
void	add_entry(char *entry, char **envp);
int		print_array(char **array);
int		unset_variable(int i, char **envp);

/* parser.c */
t_cmd	*parser(char *input, t_data *data);
t_tok	*lexer(char *input, lex_state state, t_tok *tail, t_data *data);
int		process_tokens(t_tok *token, int *numargs, int *numredir);
t_tok	*check_syntax(t_tok *head);
t_cmd 	*generate_structs(t_tok *head, int numargs, int numredir);

/* lexer_mid_fts.c */
t_tok	*check_word_border(lex_state *state, t_tok *tail, char c, int *err);
void	handle_quotes(lex_state *state, t_tok *tail, char c);
t_tok	*handle_special(t_tok *tail, char c, int *err);
int		handle_expand(char *start, t_tok *tail, t_data *data, int *err);
int		change_word(t_tok *token, char *var, char *start);

/* lexer_edge_fts.c */
t_tok	*set_start(t_tok *tail, t_tok **head, int len, int *err);
t_tok	*gen_token(t_toktype type, int len, int *err);
t_tok	*set_end(lex_state *state, t_tok *tail, t_tok *head, int *err);
t_tok	*free_tokens(t_tok *head);
void	print_toktype(t_tok *token);

/* process_tok_fts.c */
int		handle_notpipe(t_tok *token, int cmd, int *numredir);
int		io_type(t_tok *token, t_toktype type, int *numredir);
int		get_heredoc(t_tok *head, t_tok *tail);
int		open_tmp_file(char **name);
void	get_input(int fd, char *limiter, size_t len);

/* gen_struct_fts.c */
t_cmd	*init_struct(int numargs, int numredir, int *err);
int		fill_struct(t_tok *head, t_cmd *cmd, int *idx_a, int *idx_r);
t_cmd	*free_cmd(t_cmd *cmd);

#endif
