


#ifndef HELPER_H
#define HELPER_H

typedef struct s_env
{
    char **env_arr;
    char **undeclared;
    int size;
    int size_undec;
}t_env;

int     equal_strcmp(const char *s1, const char *s2);
int     compare_key(char **array, char *key);
void    print_export(t_env *e);
void    f_export(char **cmd, t_env *ev);
void    ft_putstr(char *str);
int     size_2d(char **arr);
void    *free_array(char **s);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);
void    f_cd(char *path, char *env[]);
void    f_pwd(void);
void    f_echo(char **av);
void    f_env(char *env[]);
t_env   init_env(char *ev[]);

#endif