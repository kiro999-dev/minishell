/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:28:32 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/13 02:12:18 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
typedef enum e_TOKENS
{
	PIPE
	,REDIR_OUT,REDIR_IN,DOLLAR,CMD,WORD,APPEND
	,HER_DOC,IS_FILE_APPEND,IS_FILE_IN,IS_FILE_OUT,LIMTER,eq
}t_TOKENS;

typedef struct s_toknes
{
	t_TOKENS        type;
	char*           val;
	struct s_toknes *next;
	struct s_toknes *prv;
	int				split_it;
	char			*joined_str;
}t_toknes_list;


typedef struct s_tok_data
{
	char	*token;
	int		is_cmd;
	int 	is_here_d;
	int		is_redir_in;
	int		is_redir_out;
	int		is_append;
	int		q;
	int		dq;
}t_tok;
typedef struct s_file
{
	char *file;
	t_TOKENS type;
	struct s_file *next;
	
}t_file;

typedef struct s_exc_lits
{
	char 	**cmd;
	t_TOKENS type;
	t_file	*head_files;
	char 	*limiter;
	int		here_doc;
	struct s_exc_lits *next;
	
}t_exc_lits;

char   	**ft_split(const char *s, char c);
char    *join_character(char *s,char c);
char	*ft_strdup(const char *s1);
int     ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
void  	lex(char *s, t_toknes_list **head,int flag);
void	single_q(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head);
void 	double_q(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head);
int 	pipe_symbol(int *i_ptr,int *is_cmd,t_toknes_list **head);
int		redir_out(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
int 	redir_in(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
void	add(t_toknes_list **head,char *val,t_TOKENS type);
void 	check_syntax(t_toknes_list *head);
int 	check_expand(t_toknes_list *head,char **env);
void 	expanding(t_toknes_list *head,char **env);
void 	add_list_file(t_file **head, char *name, t_TOKENS type);
void 	add_list_exc(t_exc_lits **head, 
			char **cmd, t_TOKENS type, t_file *head_files);
void 	add_back_list(t_exc_lits **head,t_exc_lits *node);
t_exc_lits *creat_node_exc(char **cmd, t_TOKENS type, t_file *head_files,char *limiter);
void 	print(char *s , t_TOKENS type);
void    print_lits(t_toknes_list *head);
char	*ft_substr(char const *s,  int start, int len);
int 	ft_isspace(int c);
char	**ft_split(const char *s, char c);
size_t	counting_words(const char *s1, char c);

// char	*ft_strjoin(char const *s1, char const *s2);
#endif
