#include "../minishell.h"
#include "heper.h"

char *get_path(char *av[], char *key, int len)
{
    int i;

    i = 0;
    if (!av || !key)
        return (NULL);
    while (av[i])
    {
        if (!ft_strncmp(av[i], key, len))
            return (ft_strdup(av[i]));
        i++;
    }
    return (NULL);
}

void replace_key_value(char *av[], char *key, char *value)
{
    int i;
    int len;

    i = 0;
    if (!av || !key || !value)
        return ;
    len = ft_strlen(key);
    while (av[i])
    {
        if (!ft_strncmp(av[i], key, len))
        {
            av[i] = value;
            break;
        }
        i++;
    }
}


void verify_path(char *env[], char *path, char *current, char *old)
{
    char *temp;

    if (!ft_strncmp(path, "-", 2))
    {
        old = get_path(env, "OLDPWD=", 7) + 7;
        if (chdir(old) == -1)
           ( printf("file or directory not found!"), exit(1));
        temp = ft_strjoin("OLDPWD=", current);
        replace_key_value(env, "OLDPWD", temp);
        free(current);
        current = ft_strjoin("PWD=", old);
        replace_key_value(env, "PWD", current);
        printf("%s\n", old);
        exit(0);
    }
    if (!ft_strncmp(path, "--", 3))
    {
        path = ft_strdup("~");
        printf("~ : %s\n", path);
    }
    if (chdir(path) == -1)
    {
        printf("file or directory not found!");
        exit(1);
    }
}

void f_cd(char *path, char *env[])
{
    char *old_pwd;
    char *new_pwd;
    char *current;

    if (!env || !path)
        return ;
    
    current = getcwd(NULL, 0);
    
    verify_path(env, path, current, new_pwd);
    old_pwd = ft_strjoin("OLDPWD=", current);
    free(current);
    current = getcwd(NULL, 0);
    new_pwd = ft_strjoin("PWD=", current);
    free(current);
    replace_key_value(env, "PWD", new_pwd);
    replace_key_value(env, "OLDPWD", old_pwd);
    printf("%s\n%s\n", old_pwd, new_pwd);
}

int main(int ac, char *av[], char *env[])
{
    t_env vars;

    vars.env = env;
    if (ac > 2)
    {
        f_cd(av[2], env);
    }
    return 0;
}