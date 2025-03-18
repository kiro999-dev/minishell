/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:28:32 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 17:23:02 by onajem           ###   ########.fr       */
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
#include <limits.h>



typedef struct s_env_list
{
    char *var;
    struct s_env_list *next;
    short undec;
} t_env_list;

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
	int				split_it2;
	int				join_me;
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
	int		join_me;
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
typedef struct s_gc_collector
{
	void *ptr;
	struct s_gc_collector *next;
}t_gc_collector;

typedef struct s_data_parsing
{
	t_exc_lits *head_exe;
	t_toknes_list *head_toknez;
	t_env_list *e;
	t_gc_collector *gc_head;
	char *buff;
}t_data_parsing;

// parsing
char 	*join_character(char *s,char c);
char	*ft_strdup(const char *s1);
int     ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
void  	lex(char *s, t_toknes_list **head);
void 	single_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head);
void 	double_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head);
int 	pipe_symbol(int *i_ptr,int *is_cmd,t_toknes_list **head);
int		redir_out(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
int 	redir_in(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
void	add(t_toknes_list **head, char *val, t_TOKENS type,int join_me);
int 	check_syntax(t_toknes_list *head);
int 	check_expand(t_toknes_list *head,t_env_list *e);
int 	check_is_expandig(t_toknes_list *head, t_env_list *e);
void 	expanding(t_toknes_list *token_head,t_env_list *e);
void 	add_list_file(t_file **head, char *name, t_TOKENS type);
void 	add_list_exc(t_exc_lits **head, char **cmd, t_TOKENS type, t_file *head_files);
void 	add_back_list(t_exc_lits **head,t_exc_lits *node);
t_exc_lits *creat_node_exc(char **cmd, t_TOKENS type, t_file *head_files,char *limiter);
void 	print(char *s , t_TOKENS type);
void    print_lits(t_toknes_list *head);
char	*ft_substr(char const *s, int start, int len);
int 	ft_isspace(int c);
char	**ft_split(const char *s, char *c);
size_t	counting_words(const char *s1, char *c);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strjoin(char const *s1, char const *s2);
void 	*gc_malloc(size_t size);
void	free_gc(t_gc_collector **gc_head);



// execution
void    f_exit(char **cmd, t_data_parsing *data_exe);
void    f_unset(t_env_list **env, char **var);
char	*ft_strdup(const char *s1);
char    *trim_plus_sign(char *key);
void	*ft_memcpy(void *dst, const void *src, size_t n);
t_env_list *init_env(char *ev[]);
int     equal_strcmp(const char *s1, const char *s2);
void    print_export(t_env_list *e);
void    f_export(char **cmd, t_env_list *ev);
void    export_putstr(char *str);
int     size_2d(char **arr);
void    *free_array(char **s);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
void    f_cd(char *path, t_env_list *env);
void    f_pwd(void);
void    f_echo(char **av);
void    f_env(t_env_list *env);
char	*custom_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_substr(char const *s, int start, int len);
void execution(t_data_parsing *data_exec);

void free_env_list(t_env_list *env);
void add_back(t_env_list **lst, t_env_list *new);
t_env_list	*new_node(void *content);


#endif
