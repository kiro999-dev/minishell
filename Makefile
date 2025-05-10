SRC = parsing/minishell.c parsing/lexer.c parsing/strings.c parsing/linked_lists.c parsing/tokenizer_list.c \
 parsing/check_syntax.c parsing/ft_substr.c parsing/ft_split.c parsing/ft_strjoin.c parsing/garbge_collcter.c \
 parsing/parsing_cmd.c parsing/parsing_cmd_utlis1.c  parsing/parsing_cmd_utlis2.c parsing/lexer2.c parsing/lexer1.c parsing/lexer3.c \
 parsing/genrate_exe_lits.c parsing/expanding2.c \
 parsing/expanding1.c parsing/expanding_utilis.c  parsing/expanding3.c \
 execution/cd.c execution/echo.c execution/export.c execution/strings_2.c execution/env_utils.c execution/unset.c \
 execution/exit.c execution/executor.c  execution/signals.c parsing/expanding_here_doc1.c parsing/expanding_here_doc2.c \
 execution/files_utils.c execution/herdoc.c execution/exe_utils.c execution/itoa.c execution/ft_atoi.c execution/cd_utils.c \
 execution/list_env.c execution/export_utils.c execution/exit_status.c execution/strings_3.c execution/pipes.c

OBJ = $(SRC:.c=.o)
INC = minishell.h 
CC = cc
CFLAGS = -Wall -Wextra -Werror 

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