NAME = philo

CC = gcc

CFLAGS = -Wall -Werror -Wextra -g -pthread

CFILES = main.c init.c utils.c \
			threads.c tasks.c \

OBJECTS = $(CFILES:.c=.o)

all: $(NAME)

%.o : %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

clean:
	@rm -f $(OBJECTS)


fclean: clean
	@rm -f $(NAME)


re: fclean all

norm:
	norminette ft_printf utils checker

#run:	all

.PHONY: all clean fclean re norm