/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:56:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/15 10:07:57 by eusatiko         ###   ########.fr       */
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

typedef enum e_toktype
{
	UNDETERM,
	END,
	SQERR,
	DQERR,
	PIPERR,
	PIPE,
	CMD,
	ARGS,
	IOTYPE,
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

typedef enum e_lex_state
{
	DELIM,
	EXPAND,
	WORD,
	INSQTS,
	INDQTS,
	INREDIR,
	OUTREDIR
}	t_lex_state;

typedef struct s_lex
{
	t_lex_state	*state;
	char		*input;
	t_tok		*tail;
	t_tok		*head;
	struct data	*data;
	int			err;
	char		lastchar;
}	t_lex;

typedef struct pids
{
	int			pid;
	struct pids	*next;
}	t_pids;

typedef struct data
{
	struct cmd			*cmd;
	t_pids				*pids;
	int					st_code;
	int					fd[2];
	int					child;
	int					std_in;
	char				**envp;
	char				**paths;
	char				*tty_out;
	struct sigaction	*sa;
	struct sigaction	*sa_child;
	struct sigaction	*sa_ex;
	struct sigaction	*sa_quit;
	struct sigaction	*sa_quit_ex;
	struct sigaction	*sa_quit_child;
}	t_data;

typedef enum cmd_check
{
	BLTN,
	NSCHFL,
	CMDNF,
	ISDIR,
	BIN
}	t_cmd_check;

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
	t_toktype	type;
	char		*value;
}	t_redirect;

extern volatile sig_atomic_t	lastsignal;

/* main.c */
void	main_loop(t_data *data, t_tok **head);
t_tok	*read_input(t_data *data);
t_tok	*accept_multiline_input(t_tok *tail, char **input, t_data *data);

/* parser.c */
t_cmd	*parser(t_tok *head, t_data *data);
t_tok	*lexer(char *input, t_tok *tail, t_data *data);
int		check_syntax(t_tok *head, t_tok **end, t_data *data);
int		multi_pipe_check(t_tok *head);
t_tok	*free_tokens(t_tok *head);

/* lexer_edge_fts.c */
t_lex	*init_lex(char *input, t_tok *tail, t_data *data);
void	set_start(t_lex *lex, int len);
t_tok	*gen_token(t_toktype type, int len, t_lex *lex);
void	extend_word(t_lex *lex, int len);
t_tok	*set_end(t_lex *lex);

/* lexer_mid_fts.c */
void	check_word_border(t_lex *lex, char c);
void	handle_quotes(t_lex *lex, char c);
void	handle_special(t_lex *lex, char c);
void	handle_pipe(t_lex *lex);

/* expand.c */
int		handle_expand(t_lex *lex, int dist);
char	*find_var(char **list, char *start, int i);
int		change_word(t_tok *token, char *var, char *start, t_lex_state *state);

/* process_tokens.c */
void	process_tokens(t_tok *token, int *numargs, int *numredir);
void	handle_iotype(t_tok *token, int *numredir);

/* get_heredoc.c */
int		get_heredoc(t_tok *head, t_tok *tail, t_data *data);
int		hd_found(t_tok *curr, t_tok *head, t_data *data);
int		open_tmp_file(char **name);
int		hd_parent(int fd, int pid, t_tok *token, char *name);

/* get_hd_input.c */
int		hd_child(int fd, t_tok *token, t_tok *head, t_data *data);
int		get_input(char **line, char *word, int fd, t_data *data);
int		expand_and_write(int fd, char *line, t_data *data);
int		handle_hd_expand(char *start, t_tok *token, t_data *data, int *err);

/* generate_structs.c */
t_cmd	*generate_structs(t_tok *head, int numargs, int numredir);
t_cmd	*init_struct(int numargs, int numredir, int *err);
int		fill_struct(t_tok *head, t_cmd *cmd, int *idx_a, int *idx_r);
t_cmd	*free_cmd(t_cmd *cmd, int i);
void	free_redirs(t_redirect *redir);

/* execution.c */
int		execute_loop(t_data *data);
void	wait_loop(t_data *data);
int		fork_function(t_cmd *cmd, t_data *data);
int		child_process(t_cmd *cmd, t_data *data);
void	exec_child(t_cmd *cmd, t_data *data);

/* redirect.c */
int		redirect(t_cmd *cmd, t_data *data);

/* iterative_shlvl.c */
void	iterate_shlvl(t_data *data);

/* check_command.c */
int		check_command(t_cmd *cmd, t_data *data);
int		find_binary(t_data *data, t_cmd *cmd);
int		check_path(t_cmd *cmd);
int		parce_paths(t_data *data, t_cmd *cmd);
int		path_not_found(t_cmd *cmd, t_data *data);

/* inits.c */
void	init_data(t_data *data, char **envp);
void	handle_sigint(int sig);
char	**dup_envp(char **envp);
int		new_pid(int pid, t_data *data);

/* clean_exit.c */
int		clean_exit(char *msg, int r_value, t_data *data);
char	**free_paths(char **paths);
t_pids	*free_pids(t_pids *pids);
t_cmd	*free_cmds(t_cmd *cmd_list);
char	**free_envp(char **envp);

/* builtins folder */
int		check_builtin(t_cmd *cmd, t_data *data);
void	ft_echo(t_cmd *cmd, t_data *data);
int		ft_unset(t_cmd *cmd, t_data *data);
void	ft_cd(t_cmd *cmd, t_data *data);
void	ft_exit(t_cmd *cmd, t_data *data);
int		ft_export(char *arg, t_cmd *cmd, t_data *data);

/* ft_export_utils.c */
int		print_array(char **array);
int		find_key(t_export *ex, char **envp, t_data *data);
int		add_entry(char *entry, char **envp, t_data *data);
int		unset_variable(int i, char **envp, t_data *data);

#endif
