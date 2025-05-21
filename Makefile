CC      := cc
CFLAGS  := -Wall -Wextra -Werror
NAME    := minishell

SRCS    := minishell.c signal.c execution/ft_execute.c execution/ft_utils.c execution/multi_pipes.c execution/pipex.c execution/return.c
OBJS    := $(SRCS:.c=.o)

LIBFT_DIR := ./libft
LIBFT_A   := $(LIBFT_DIR)/libft.a

LDFLAGS := -lreadline -lncurses

GREEN := \033[1;32m
BLUE  := \033[1;34m
RED   := \033[1;31m
RESET := \033[0m

all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)[Linking]$(RESET) $@"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -o $@ $(LDFLAGS)
	@echo "$(GREEN)[Done]$(RESET) Built $(NAME)"

%.o: %.c
	@echo "$(BLUE)[Compiling]$(RESET) $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	@echo "$(BLUE)[Building Libft]$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "$(RED)[Cleaning]$(RESET) object files"
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)[Cleaning]$(RESET) executable"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

