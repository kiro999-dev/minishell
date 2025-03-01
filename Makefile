SRC = minishell.c lexer.c strings.c ft_split.c 
OBJ = $(SRC:.c=.o)
INC = minishell.h
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline            
NAME = minishell


all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)  


%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $< -o $@ 


clean:
	rm -f $(OBJ)


fclean: clean
	rm -f $(NAME) 


re: fclean all
