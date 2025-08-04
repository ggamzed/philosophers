SRCS	=			init.c main.c mutex_utils.c simulation.c exit_utils.c utils.c sim_utils.c

OBJS	=			$(SRCS:.c=.o)
CC		=			cc
RM		=			rm -f
CFLAGS	=			-Wall -Wextra -Werror
NAME	=			philo

all:				$(NAME)

$(NAME):			$(OBJS)
					$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -fsanitize=thread

clean:
					$(RM) $(OBJS)

fclean:	clean
					$(RM) $(NAME)
					
re:		fclean all

.PHONY:	all clean fclean re