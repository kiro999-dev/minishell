/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:28:32 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/07 18:40:12 by zkhourba         ###   ########.fr       */
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
# include <fcntl.h>
# include <sys/wait.h>
#include <signal.h>

extern int g_status;

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
	int				ambiguous;
	int 			len_expand;
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
typedef struct s_list_here_doc
{
	char	*limtter;
	int		here_doc;	
	struct s_list_here_doc *next; 
	t_TOKENS type;
}t_list_here_doc;

typedef struct s_exc_lits
{
	char 	**cmd;
	t_TOKENS type;
	t_file	*head_files;
	char 	*limiter;
	t_list_here_doc *head_here_doc;
	int		in;
	int		out;
	char  *heredoc_filename;
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
	t_file *head_file;
}t_data_parsing;


void	handler(int sig);
void	signals_handling(void);
void 	heredoc_signals(void);
int set_herdoc_delimeter(int exit, int flag);
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
int 	split_t2condi(char *s,int i);
int 	check_expand(t_toknes_list *head,t_env_list *e);
int 	check_is_expandig(t_toknes_list *head, t_env_list *e);
void 	expanding(t_toknes_list *token_head,t_env_list *e);
void 	add_list_file(t_file **head, char *name, t_TOKENS type);
void 	add_list_exc(t_exc_lits **head, char **cmd, t_TOKENS type, t_file *head_files);
void 	add_back_list(t_exc_lits **head,t_exc_lits *node);
t_exc_lits	*creat_node_exc(char **cmd, t_TOKENS type,
	t_file *head_files);
char	*ft_substr(char const *s, int start, int len);
int 	ft_isspace(int c);
char	**ft_split(const char *s, char *c);
size_t	counting_words(const char *s1, char *c);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strjoin(char const *s1, char const *s2);
void 	*gc_malloc(size_t size,int flag);
void	free_gc(t_gc_collector **gc_head);
int 	is_token(char c);
int 	isparam_for_file(t_tok *data_tok);
int 	isfile(t_TOKENS type);
int 	ft_isspace(int c);
void 	token_add(t_toknes_list **head,t_tok *data_tok,t_TOKENS type);
void 	add_token(t_tok *data_tok,t_toknes_list **head);
void 	generate_list(t_toknes_list *tokenz_head, t_exc_lits **exc_head);
void	init_tok(t_tok *data_tok);
void 	not_token_case(t_tok *data_tok,t_toknes_list **head,int *ptr_i,char *s);
void  	lex(char *s, t_toknes_list **head);
void 	gen_word(t_tok  *data_tok, char *s,int *ptr_i);
void 	single_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head);
void 	double_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head);
int 	pipe_symbol(int *i_ptr,int *is_cmd,t_toknes_list **head);
int 	redir_in(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
int 	redir_out(int *i_ptr,char *s,t_toknes_list **head,t_tok *d);
int 	count_cmd_tokens(t_toknes_list *token);
int		counting(char **spilt);
char	 **realloc_cmd_array(char **cmd, int current_count, int extra);
char 	**cmd_int(int count);
char 	**pars_cmd(t_toknes_list *token);
char 	**copy_cmd_tokens(t_toknes_list *token, int count);
void 	process_default(t_toknes_list **current, char **cmd, int *i);
char 	**process_split_it2(t_toknes_list **current, char **cmd, int *i, int orig_count);
void	 process_split_it(t_toknes_list **current, char **cmd, int *i);
char	*expand_val(char *s, t_toknes_list *head, int j, int flag);
void 	handle_dollar_expansion(int *i, t_env_list  *e, t_toknes_list *head, int flag3);
void 	remove_q_d(t_toknes_list *head);
int 	not_character_expand(char c);
void 	skip_q_expand(char *s, int *ptr_i, int *q_ptr);
int		strcmp_env(char *s1, char *s2, int n);
int 	ft_strcmp(char *s1 ,char *s2);
void	join_the_strings(t_toknes_list **current, char **cmd, int *i);
void 	copy_the_splited_string(char **split,char **cmd,int *i,int j);
void	here_doc_add(t_list_here_doc **head, char *LIMTER, t_TOKENS type, int here_doc);
char	*expand_val_h(char *s, char *val, int j, int flag);
int		check_is_expandig_h(char **val, t_env_list *e);
void	handle_dollar_expansion_h(int *i, t_env_list *e, char **val);
int	check_expand_h(char **val, t_env_list *e);
// execution
char	*ft_itoa(int n);
char *find_path(t_env_list *env, char *key, int len);
int	env_size(t_env_list *lst);
int	cmds_size(t_exc_lits *lst);
int	ft_isalnum(int ch);
int	ft_isalpha(int c);
void    f_exit(char **cmd, t_data_parsing *data_exe);
void    f_unset(t_env_list **env, char **var);
char	*ft_strdup(const char *s1);
char    *trim_plus_sign(char *key);
void	*ft_memcpy(void *dst, const void *src, size_t n);
t_env_list *init_env(char *ev[]);
int     equal_strcmp(const char *s1, const char *s2);
void    print_export(t_env_list *e);
void f_export(char **cmd, t_env_list **ev);
void    export_putstr(char *str);
int     size_2d(char **arr);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
void f_cd(char **cmd, t_env_list **env);
void	echo_putstr(char *s);
void f_pwd(t_env_list *env);
void    f_echo(char **av);
void    f_env(t_env_list *env);
char	*custom_strnstr(const char *haystack, const char *needle, size_t len);
void	execution(t_data_parsing *data_exec);
void add_back(t_env_list **lst, t_env_list *new);
t_env_list	*new_node(void *content);


#endif
