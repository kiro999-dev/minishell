/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:28:32 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/01 03:49:36 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
typedef enum e_TOKENS
{
	PIPE
	,REDIR_OUT,REDIR_IN,DOLLAR,CMD,WORD,APPEND
	,HER_DOC,IS_FILE_APPEND,IS_FILE,LIMTER
}t_TOKENS;

typedef struct s_toknes
{
	t_TOKENS        type;
	char*           val;
	struct s_toknes *next;
	struct s_toknes *prv;
}t_toknes_list;

typedef struct s_ast {
    char **cmd;
    char *input_file;
    char *output_file;
    int append_mode;
	char* val;
    t_TOKENS type;
    struct s_ast *left;
    struct s_ast *right;
} t_ast;
typedef struct s_tok_data
{
	char	*token;
	int		is_cmd;
	int 	is_here_d;
	int		is_redir;
	int		is_append;
	int		q;
	int		dq;
}t_tok;

char   	**ft_split(const char *s, char c);
char    *join_character(char *s,char c);
char	*ft_strdup(const char *s1);
int     ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
void    lex(char *s, t_toknes_list **head);
void	single_q(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head);
void 	double_q(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head);
int 	pipe_symbol(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head);
int		redir_out(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
int 	redir_in(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
void	add(t_toknes_list **head,char *val,t_TOKENS type);
void 	print(char *s , t_TOKENS type);
void    print_lits(t_toknes_list *head);
#endif
