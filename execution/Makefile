
SRC = main.c cd.c echo.c export.c strings_2.c env_utils.c ../strings.c unset.c ../garbge_collcter.c

OBJ = $(SRC:.c=.o)

INC = ../minishell.h heper.h
CC = cc


CFLAGS = -Wall -Wextra -Werror 
        
NAME = minishell


all: $(NAME)


$(NAME): $(OBJ)
	$(CC)  $(OBJ) $(LDFLAGS) -o $(NAME)  


%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $< -o $@ 


clean:
	rm -f $(OBJ)


fclean: clean
	rm -f $(NAME) 


re: fclean all
