/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:56:30 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 15:05:51 by auspensk         ###   ########.fr       */
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

typedef enum lex_state
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
	char			*word;
	int				idx;
	struct s_tok	*next;
	t_toktype		type;
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

typedef struct export
{
	char			*arg;
	char			*key;
	t_cmd_type		type;
}	t_export;

typedef enum cmd_type
{
	UNSET,
	EXPORT
} t_cmd_type;

int		clean_exit(char *msg, int r_value, t_data *data);
t_cmd	*parce(void);
int		redirect(t_cmd *cmd);
int		new_pid(int pid, t_data *data);
int		check_command(t_cmd *cmd, t_data *data);
int		check_builtin(t_cmd *cmd, t_data *data);
int		ft_echo(t_cmd *cmd, t_data *data);
// void	read_input(t_data *data);
int		execute_loop(t_data *data);
void	init_data(t_data *data, char **envp);
t_cmd	*parser(char *input, char **envp);
t_tok	*lexer(char *input, lex_state state, t_tok *tail, char **envp);
t_tok	*gen_token(t_toktype type, int len);
char	*expand(char *start, int *lenvar, char **envp);
void	change_word(t_tok *token, char *var, int len);
int		process_tokens(t_tok *head);
int		io_type(t_tok *token, t_toktype type);
void	insert_token(t_tok *token);
int		check_syntax(t_tok *head);
t_cmd	*generate_structs(t_tok *head, int numargs);
int		ft_export(t_cmd *cmd, t_data *data);
int		print_array(char **array);

#endif
