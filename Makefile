SRC = minishell.c lexer.c strings.c linked_lists.c tokenizer_list.c expanding.c check_syntax.c ft_substr.c ft_split.c ft_strjoin.c
OBJ = $(SRC:.c=.o)
INC = minishell.h
CC = cc
CFLAGS = -Wall -Wextra -Werror -g   

LDFLAGS = -lreadline            
NAME = minishell


all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)  


%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $< -o $@ 


clean:
	rm -f $(OBJ)


fclean: clean
	rm -f $(NAME) 


re: fclean all