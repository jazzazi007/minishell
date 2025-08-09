# Compiler and Flags
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -g
LDFLAGS := -lreadline -lncurses

# Project Name
NAME    := minishell

# Source and Object Files
SRCS := \
	src/debug.c							\
	src/environment.c 					\
	src/error.c 						\
	src/ft_free.c						\
	src/ft_utils.c 						\
	src/minishell.c 					\
	src/return.c 						\
	src/signal.c    					\
	src/built-ins/echo.c				\
	src/built-ins/pwd.c					\
	src/parsing/get_cmd.c 				\
	src/execution/ft_execute.c 			\
	src/execution/pipex.c 				\
	src/execution/multi_pipes.c 		\
	src/execution/init_fork.c 			\
	src/execution/init_pipes.c 			\
	src/parsing/builed_cmd.c 			\
	src/parsing/expander.c 				\
	src/parsing/exp_utils.c 			\
	src/parsing/heredoc.c 				\
	src/parsing/parse_cmd.c 			\
	src/parsing/parse_redirections.c 	\
	src/parsing/shell_utils.c			\
	src/parsing/tokenizer.c 			\
	src/parsing/token_utils.c 			\
	

OBJ_DIR := obj
OBJS := $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# Libft
LIBFT_DIR := libft
LIBFT_A   := $(LIBFT_DIR)/libft.a

# Colors
BLUE := \033[1;34m
RESET := \033[0m

# Targets
all: $(OBJ_DIR) $(LIBFT_A) $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -o $@ $(LDFLAGS)
	@echo "$(BLUE)MAKE SHELLX DONE$(RESET)"

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) --silent

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --silent

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --silent

re: fclean all

.PHONY: all clean fclean re
