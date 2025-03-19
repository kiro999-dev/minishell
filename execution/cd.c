#include "../minishell.h"


void replace_key_value(t_env_list *env, char *key, char *value)
{
    int len;

    if (!env || !key || !value)
        return ;
    len = ft_strlen(key);
    while (env)
    {
        if (!ft_strncmp(env->var, key, len))
        {
            env->var = value;
            break;
        }
        env = env->next;
    }
}


void verify_path(t_env_list *env, char *path, char *current, char *old)
{
    char *temp;

    if (!ft_strncmp(path, "-", 2))
    {
        old = getenv("OLDPWD");
        if (chdir(old) == -1)
        {
            printf("Xminishell: cd: %s: No such file or directory\n", old);
            return ;
        }
        temp = ft_strjoin("OLDPWD=", current);
        replace_key_value(env, "OLDPWD", temp);
        free(current);
        current = ft_strjoin("PWD=", old);
        replace_key_value(env, "PWD", current);
        printf("%s\n", old);
        return ;
    }
    if (!ft_strncmp(path, "--", 3))
        path = getenv("HOME");
    if (chdir(path) == -1)
        printf("minishell: cd: %s: No such file or directory\n", path);
}

void f_cd(char **cmd, t_env_list *env)
{
    char *old_pwd;
    char *new_pwd;
    char *current;
    int size;

    if (!env || !cmd)
        return ;
    size = size_2d(cmd);
    if (size > 2)
    {
        printf("bash: cd: too many arguments\n");
        return ;
    }
    else if (size == 1)
    {   
        return  ;
    }
    current = getcwd(NULL, 0);
    new_pwd = NULL;
    verify_path(env, cmd[1], current, new_pwd);
    old_pwd = ft_strjoin("OLDPWD=", current);
    current = getcwd(NULL, 0);
    new_pwd = ft_strjoin("PWD=", current);
    replace_key_value(env, "PWD", new_pwd);
    replace_key_value(env, "OLDPWD", old_pwd);
}

void f_pwd(void)
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    if (!pwd)
        return ;
    printf("%s\n", pwd);
}

void f_env(t_env_list *env)
{
    if (!env)
    {
        printf("env not found!\n");
        return ;
    }
    while (env && !env->undec)
    {
        printf("%s\n", env->var);
        env = env->next;
    }
}