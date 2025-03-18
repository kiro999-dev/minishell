SRC = parsing/minishell.c parsing/lexer.c parsing/strings.c parsing/linked_lists.c parsing/tokenizer_list.c \
 parsing/expanding.c parsing/check_syntax.c parsing/ft_substr.c parsing/ft_split.c parsing/ft_strjoin.c parsing/garbge_collcter.c \
 execution/cd.c execution/echo.c execution/export.c execution/strings_2.c execution/env_utils.c execution/unset.c \
 execution/exit.c


OBJ = $(SRC:.c=.o)
INC = parsing/minishell.h execution/heper.h 
CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
#  -fsanitize=address
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