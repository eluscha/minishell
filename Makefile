NAME = minishell

CC = cc

SRC = execution.c \
		parser.c \
		lexer_mid_fts.c \
		lexer_edge_fts.c \
		process_tok_fts.c \
		gen_struct_fts.c \
		utils.c \
		builtins.c \
		check_path.c \
		main.c \
		redirect.c \
		inits.c \

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
