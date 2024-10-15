NAME = minishell

CC = cc

SRC = execution.c \
		parser.c \
		lexer_mid_fts.c \
		expand.c \
		lexer_edge_fts.c \
		process_tokens.c \
		generate_structs.c \
		get_heredoc.c \
		get_hd_input.c \
		clean_exit.c \
		check_command.c \
		main.c \
		redirect.c \
		inits.c \
		iterate_shlvl.c \
		builtins/builtins.c \
		builtins/ft_export.c \
		builtins/ft_export_utils.c \
		builtins/ft_exit.c \
		builtins/ft_cd.c \
		builtins/ft_echo.c

OBJS = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g

INCL = -Ilibft/ -I/usr/include

LIBFT_PATH = libft/

LIBFT_NAME = libft.a

LIBFT = $(LIBFT_PATH)$(LIBFT_NAME)

.PHONY: all clean fclean re

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) minishell.h
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) $(INCL) -lreadline

$(LIBFT):
	@make -sC $(LIBFT_PATH)

%.o: %.c
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make clean -C $(LIBFT_PATH)

fclean: clean
	rm -f $(NAME)
	@make fclean -C $(LIBFT_PATH)

re: fclean all
