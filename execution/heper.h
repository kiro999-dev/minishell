


#ifndef HELPER_H
#define HELPER_H

#include <unistd.h>
#include <limits.h>



typedef struct s_env_list
{
    char *var;
    struct s_env_list *next;
    short undec;
} t_env_list;


void f_unset(t_env_list **env, char **var);
char	*ft_strdup(const char *s1);
char *trim_plus_sign(char *key);
void	*ft_memcpy(void *dst, const void *src, size_t n);
t_env_list *init_env(char *ev[]);
int     equal_strcmp(const char *s1, const char *s2);
// int     compare_key(char **array, char *key);
void    print_export(t_env_list *e);
void    f_export(char **cmd, t_env_list *ev);
void    export_putstr(char *str);
int     size_2d(char **arr);
void    *free_array(char **s);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
// char	*ft_strjoin(char *s1, char *s2);
void    f_cd(char *path, t_env_list *e);
void    f_pwd(void);
void    f_echo(char **av);
void    f_env(t_env_list *env);
char	*custom_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_substr(char const *s, int start, int len);
// size_t	ft_strlcat(char *dest, const char *src, size_t size);


// env linked list functions
void free_env_list(t_env_list *env);
void add_back(t_env_list **lst, t_env_list *new);
t_env_list	*new_node(void *content);
#endif