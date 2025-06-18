
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
NAME    = libft.a
ARFLAGS = rcs

SRCS = ft_isalpha.c ft_strcmp.c ft_bzero.c ft_isalnum.c ft_isascii.c \
	   ft_isdigit.c ft_isprint.c ft_strlen.c ft_memset.c ft_strlcpy.c \
	   ft_toupper.c ft_tolower.c ft_strlcat.c ft_memcpy.c ft_strchr.c \
	   ft_strrchr.c ft_strncmp.c ft_strnstr.c ft_atoi.c ft_memmove.c \
	   ft_memchr.c ft_memcmp.c ft_calloc.c ft_strdup.c ft_substr.c \
	   ft_strjoin.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
	   ft_putnbr_fd.c ft_striteri.c ft_strmapi.c ft_strtrim.c ft_itoa.c \
	   ft_split.c get_next_line.c get_next_line_utils.c ft_strtok.c \
	   ft_strcat.c ft_strcpy.c 

OBJS = $(SRCS:.c=.o)

GREEN = \033[1;32m
RESET = \033[0m

all: $(NAME)
	@echo "$(GREEN)MAKE LIBFT DONE$(RESET)"

$(NAME): $(OBJS)
	@ar $(ARFLAGS) $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re




