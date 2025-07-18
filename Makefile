# Compiler and Flags
CC      := cc
CFLAGS  := -Wall -Wextra -Werror -g
LDFLAGS := -lreadline -lncurses

# Project Name
NAME    := minishell

# Source and Object Files
SRCS := \
	src/minishell.c src/signal.c src/ft_execute.c src/ft_utils.c \
	src/multi_pipes.c src/echo.c src/return.c src/pwd.c src/pipex.c \
	src/get_cmd.c src/init_fork.c src/init_pipes.c src/tokenizer.c \
	src/builed_argv.c src/token_utils.c src/error.c

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
