


#ifndef HELPER_H
#define HELPER_H

typedef struct s_env
{
    char **env;
}t_env;

int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);
#endif