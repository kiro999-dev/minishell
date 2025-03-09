


#ifndef HELPER_H
#define HELPER_H

typedef struct s_env
{
    char **env_arr;
    int size;
}t_env;

int size_2d(char **arr);
void    *free_array(char **s);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);
void    f_cd(char *path, char *env[]);
void    f_pwd(void);
void    f_echo(char **av);
void    f_env(char *env[], int export);
t_env   init_env(char *ev[]);

#endif