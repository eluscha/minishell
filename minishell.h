/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:56:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/01 16:17:24 by auspensk         ###   ########.fr       */
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
# include <signal.h>

typedef enum e_lex_state
{
	DELIM,
	WORD,
	INSQTS,
	INDQTS
}	t_lex_state;

typedef enum e_toktype
{
	UNDETERM,
	NOSPECIAl,
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
	APPEND,
	HDDASH
}	t_toktype;

typedef struct s_tok
{
	char			*word;
	int				idx;
	struct s_tok	*next;
	t_toktype		type;
}	t_tok;

typedef enum cmd_check
{
	BLTN,
	NSCHFL,
	CMDNF,
	ISDIR,
	BIN
}	t_cmd_check;

typedef struct cmd
{
	char			*cmd;
	char			**args;
	struct redirect	*redirect;
	struct cmd		*next;
	t_cmd_check		cmd_check;
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
	t_cmd				*cmd;
	t_pids				*pids;
	int					st_code;
	int					fd[2];
	int					child;
	int					std_in;
	char				**envp;
	char				**paths;
	char				*tty_in;
	char				*tty_out;
	struct sigaction	*sa;
	struct sigaction	*sa_child;
	struct sigaction	*sa_ex;
	struct sigaction	*sa_quit;
	struct sigaction	*sa_quit_ex;
	struct sigaction	*sa_quit_child;
}	t_data;

typedef enum exp_type
{
	UNSET,
	EXPORT
}	t_exp_unset;


typedef struct export
{
	char			*arg;
	char			*key;
	t_exp_unset		type;
}	t_export;

extern int lastsignal;

void	free_cmds(t_cmd	*cmd_list);
int		clean_exit(char *msg, int r_value, t_data *data);
int		redirect(t_cmd *cmd, t_data *data);
int		new_pid(int pid, t_data *data);
int		check_command(t_cmd *cmd, t_data *data);
int		execute_loop(t_data *data);
char	**dup_envp(char **envp);
void	init_data(t_data *data, char **envp);
int		path_not_found(t_cmd *cmd, t_data *data);

/*builtins*/
int		check_builtin(t_cmd *cmd, t_data *data);
void	ft_echo(t_cmd *cmd, t_data *data);
int		ft_unset(t_cmd *cmd, t_data *data);
int		find_key(t_export *ex, char **envp, t_data *data);
int		add_entry(char *entry, char **envp, t_data *data);
int		unset_variable(int i, char **envp, t_data *data);
int		ft_export(char *arg, t_cmd *cmd, t_data *data);
int		print_array(char **array);
void	ft_cd(t_cmd *cmd, t_data *data);
void	ft_exit(t_cmd *cmd, t_data *data);

t_tok	*read_input(t_data *data);

/* parser.c */
t_cmd	*parser(t_tok *head, t_data *data);
t_tok	*lexer(char *input, t_tok *tail, t_data *data);
t_tok	*check_syntax(t_tok *head);

/* lexer_mid_fts.c */
t_tok	*check_word_border(t_lex_state *state, t_tok *tail, char c, int *err);
void	handle_quotes(t_lex_state *state, t_tok *tail, char c);
t_tok	*handle_special(t_lex_state *state, t_tok *tail, char c, int *err);
int		handle_expand(char *start, t_tok *tail, t_data *data, int *err);
int		change_word(t_tok *token, char *var, char *start);

/* lexer_edge_fts.c */
t_tok	*set_start(t_tok *tail, t_tok **head, int len, int *err);
t_tok	*gen_token(t_toktype type, int len, int *err);
void	extend_word(t_tok *tail, int len, int *err);
t_tok	*set_end(t_lex_state *state, t_tok *tail, char c, int *err);
t_tok	*free_tokens(t_tok *head);
void	print_toktype(t_tok *token);

/* process_tokens.c */
int		process_tokens(t_tok *token, int *numargs, int *numredir);
int		handle_notpipe(t_tok *token, int cmd, int *numredir);
int		io_type(t_tok *token, t_toktype type, int *numredir);
int handle_hddash(t_tok *token);

/* get_heredoc.c */
int		get_heredoc(t_tok *head, t_tok *tail, t_data *data);
int		open_tmp_file(char **name);
int		get_input(int fd, t_tok *token, t_data *data);
int		expand_and_write(int fd, char *line, t_data *data);

/* generate_structs.c */
t_cmd	*generate_structs(t_tok *head, int numargs, int numredir);
t_cmd	*init_struct(int numargs, int numredir, int *err);
int		fill_struct(t_tok *head, t_cmd *cmd, int *idx_a, int *idx_r);
t_cmd	*free_cmd(t_cmd *cmd, int i);
void	print_struct(t_cmd *cmd);

/* signals */
void	init_signals(t_data *data);
void	handle_sigint(int sig);
void	handle_sigint_ex(int sig);
void	handle_sigquit_ex(int sig);

#endif
