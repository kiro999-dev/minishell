/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:28:32 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/21 20:10:29 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <signal.h>

typedef struct s_env_list
{
	char				*var;
	struct s_env_list	*next;
	short				undec;
}	t_env_list;

typedef enum e_TOKENS
{
	PIPE
	, REDIR_OUT, REDIR_IN, CMD, WORD, APPEND
	, HER_DOC, IS_FILE_APPEND, IS_FILE_IN, IS_FILE_OUT, LIMTER
}t_TOKENS;
	typedef struct s_toknes
{
	t_TOKENS type;
	char *val;
	struct s_toknes * next;
	struct s_toknes * prv;
	int split_it;
	int split_it2;
	int join_me;
	int ambiguous;
	int len_expand;
	int flag_exit;
}t_toknes_list;

typedef struct s_tok_data
{
	char	*token;
	int		is_cmd;
	int		is_here_d;
	int		is_redir_in;
	int		is_redir_out;
	int		is_append;
	int		q;
	int		dq;
	int		join_me;
}t_tok;

typedef struct s_file
{
	char			*file;
	t_TOKENS		type;
	int				ambigous;
	struct s_file	*next;

}t_file;
typedef struct s_list_here_doc
{
	char					*limtter;
	int						here_doc;	
	struct s_list_here_doc	*next;
	t_TOKENS				type;
}t_list_here_doc;

typedef struct s_exc_lits
{
	char				**cmd;
	t_TOKENS			type;
	t_file				*head_files;
	char				*limiter;
	t_list_here_doc		*head_here_doc;
	int					heredoc_fd;
	int					priority;
	struct s_exc_lits	*next;

}t_exc_lits;

typedef struct s_gc_collector
{
	void					*ptr;
	struct s_gc_collector	*next;
}t_gc_collector;

typedef struct s_data_parsing
{
	t_exc_lits		*head_exe;
	t_toknes_list	*head_toknez;
	t_env_list		*e;
	t_gc_collector	*gc_head;
	char			*buff;
	t_file			*head_file;
	char			*p_pwd;
}t_data_parsing;

typedef struct s_expand_var
{
	int				flag;
	char			*expand;
	int				found;
	t_env_list		*w;
	int				*i;
	t_env_list		*e;
	t_toknes_list	*head;
	int				flag3;
}t_expand_var;

void		print_error(const char *output, char *error, char *builtin);
int			check_here_doc_nb(t_toknes_list *head1);
int			handle_exe_files(char *cmd);
void		close_fds(void);
int			exit_status(int stat, int flag);
void		handler(int sig);
void		signals_handling(void);
void		heredoc_signals(void);
int			exit_herdoc(int x, int flag);
int			check_exit(int status);
void		default_signals(void);
size_t		counting_words(const char *s1, char *c);
char		*join_character(char *s, char c);
char		*ft_strdup(const char *s1);
int			ft_strlen(const char *s);
char		*ft_strchr(const char *s, int c);
void		lex(char *s, t_toknes_list **head);
void		single_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head);
void		double_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head);
int			pipe_symbol(int *i_ptr, int *is_cmd, t_toknes_list **head);
int			redir_out(int *i_ptr, char *s, t_toknes_list **head, t_tok *d);
int			redir_in(int *i_ptr, char *s, t_toknes_list **head, t_tok *d);
void		add(t_toknes_list **head, char *val, t_TOKENS type, int join_me);
int			check_syntax(t_toknes_list *head);
int			split_t2condi(char *s, int i);
int			check_expand(t_toknes_list *head, t_env_list *e, int flag_split);
int			check_is_expandig(t_toknes_list *head, t_env_list *e,
				int flag_split);
void		expanding(t_toknes_list *token_head, t_env_list *e);
void		add_list_file(t_file **head, char *name, t_TOKENS type,
				int ambigous);
void		add_list_exc(t_exc_lits **head, char **cmd, t_TOKENS type,
				t_file *head_files);
char		*remove_q_d_h(char *s);
void		add_back_list(t_exc_lits **head, t_exc_lits *node);
t_exc_lits	*creat_node_exc(char **cmd, t_TOKENS type, t_file *head_files);
char		*ft_substr(char const *s, int start, int len);
int			ft_isspace(int c);
char		**ft_split(const char *s, char *c);
size_t		counting_words(const char *s1, char *c);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
char		*ft_strjoin(char const *s1, char const *s2);
void		*gc_malloc(size_t size, int flag);
void		free_gc(t_gc_collector **gc_head);
int			is_token(char c);
int			isparam_for_file(t_tok *data_tok);
int			isfile(t_TOKENS type, t_toknes_list *tokenz_head);
int			ft_isspace(int c);
void		token_add(t_toknes_list **head, t_tok *data_tok, t_TOKENS type);
void		add_token(t_tok *data_tok, t_toknes_list **head);
void		generate_list(t_toknes_list *tokenz_head, t_exc_lits **exc_head);
t_exc_lits	*node_inite(void);
void		init_tok(t_tok *data_tok);
void		not_token_case(t_tok *data_tok, t_toknes_list **head, int *ptr_i,
				char *s);
void		lex(char *s, t_toknes_list **head);
void		gen_word(t_tok *data_tok, char *s, int *ptr_i);
void		single_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head);
void		double_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head);
int			pipe_symbol(int *i_ptr, int *is_cmd, t_toknes_list **head);
int			redir_in(int *i_ptr, char *s, t_toknes_list **head, t_tok *d);
int			redir_out(int *i_ptr, char *s, t_toknes_list **head, t_tok *d);
int			count_cmd_tokens(t_toknes_list *token);
int			counting(char **spilt);
char		**realloc_cmd_array(char **cmd, int current_count, int extra);
char		**cmd_int(int count);
char		**pars_cmd(t_toknes_list *token);
char		**copy_cmd_tokens(t_toknes_list *token, int count);
void		process_default(t_toknes_list **current, char **cmd, int *i);
char		**process_split_it2(t_toknes_list **current, char **cmd, int *i,
				int orig_count);
void		process_split_it(t_toknes_list **current, char **cmd, int *i);
char		*expand_val(char *s, t_toknes_list *head, int j, int flag);
void		handle_dollar_expansion(int *i, t_env_list *e, t_toknes_list *head,
				int flag3);
void		remove_q_d(t_toknes_list *head);
int			not_character_expand(char c);
void		skip_q_expand(char *s, int *ptr_i, int *q_ptr);
int			strcmp_env(char *s1, char *s2, int n);
int			ft_strcmp(char *s1, char *s2);
void		join_the_strings(t_toknes_list **current, char **cmd, int *i);
void		copy_the_splited_string(char **split, char **cmd, int *i, int j);
void		here_doc_add(t_list_here_doc **head, char *LIMTER, t_TOKENS type,
				int here_doc);
char		*expand_val_h(char *s, char *val, int j, int flag);
char		*expand_val_status_h(char *s, char *val, int j, int len_expand);
int			check_is_expandig_h(char **val, t_env_list *e, int check);
void		expand_helper(t_expand_var *data);
int			handle_dollar_expansion_h(int *i, t_env_list *e, char **val,
				int check);
int			check_expand_h(char **val, t_env_list *e);
char		*build_prefix(char *s2, int j, int n);
char		*build_env_value(char *s, int flag, int flag_exit);
char		*build_suffix(char *s2, int j);
char		*combine_parts(char *p, char *e, char *s);
void		process_quote(char *s, int *i, char **cpy, char quote);
char		*process_unquoted(char *s, int *i);
int			handle_redirection(t_exc_lits *cmd);
int			check_no_cmd(t_exc_lits *head, t_env_list *e);
void		run_command(t_env_list *e, t_exc_lits *cmd_lst, int pid);
void		child_process(t_exc_lits *cmd, t_data_parsing *data, int in,
				int fd[2]);
void		wait_multiple_childs(t_exc_lits *lst, int *pids, int cmd_len);
void		parent_process(int *prev_pipe_in, int pipe_fd[2],
				t_exc_lits **cmd_lst);
void		execute_pipeline(t_data_parsing *data_exec, int *pids,
				int cmd_size);
void		sort_env(t_env_list **head);
int			replace_existing_key(t_env_list *env, char *key);
int			valid_key(char *var);
void		add_back(t_env_list **lst, t_env_list *new);
t_env_list	*lstlast(t_env_list *lst);
void		handle_file_error(const char *path, int ex);
void		update_pwd(t_data_parsing *data, const char *old_pwd, char *new);
void		replace_key_value(t_env_list **env, const char *key,
				const char *value);
void		add_var_2_env(char *cmd, t_env_list **env);
int			is_num(char c);
int			ft_atoi(const char *nptr);
void		get_key_value(char *var, char **key, char **value);
char		*prepare_path(char *path, char *cmd);
char		*get_path(t_env_list *env, char *cmd);
char		**env_list_to_array(t_env_list *list);
int			is_builtin(char *cmd);
void		exec_builtin(t_exc_lits *cmd, t_data_parsing *data_exec, int child);
int			apply_input_redirection(int *last_input_fd, t_file *file);
int			check_in_out(t_file *file, int red);
int			apply_output_redirection(int *last_out, t_file *file, int single);
void		hexa_format(unsigned int value, char *output);
int			process_heredocs(t_exc_lits *cmd, t_env_list *e, int fd_herdoc);
void		set_final_redirections(char **cmd, int last_in_fd, int last_out_fd);
char		*generate_random_filename(void);
char		*ft_itoa(int n);
char		*find_path(t_env_list *env, char *key, int len);
int			env_size(t_env_list *lst);
int			cmds_size(t_exc_lits *lst);
int			ft_isalnum(int ch);
int			ft_isalpha(int c);
void		f_exit(char **cmd, int child);
void		f_unset(t_env_list **env, char **var);
char		*ft_strdup(const char *s1);
char		*trim_plus_sign(char *key);
void		*ft_memcpy(void *dst, const void *src, size_t n);
t_env_list	*init_env(char *ev[]);
int			equal_strcmp(const char *s1, const char *s2);
void		print_export(t_env_list *e);
void		f_export(char **cmd, t_env_list **ev, int i);
void		export_putstr(char *str);
int			size_2d(char **arr);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		f_cd(char **cmd, t_data_parsing *data);
void		echo_putstr(char *s);
void		f_pwd(t_data_parsing *data);
void		f_echo(char **av);
void		f_env(t_env_list *env, t_exc_lits *cmd);
char		*custom_strnstr(const char *haystack, const char *needle,
				size_t len);
void		execution(t_data_parsing *data_exec);
t_env_list	*new_node(void *content);

#endif
