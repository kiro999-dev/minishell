#include "../parsing/minishell.h"
#include "heper.h"

// char *get_path(char *av[], char *key, int len)
// {
//     int i;

//     i = 0;
//     if (!av || !key)
//         return (NULL);
//     while (av[i])
//     {
//         if (!ft_strncmp(av[i], key, len))
//             return (ft_strdup(av[i]));
//         i++;
//     }
//     return (NULL);
// }



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
            free(env->var);
            env->var = value;
            break;
        }
        env = env->next;
    }
}


void verify_path(t_env_list *env, char *path, char *current, char *old)
{
    char *temp;

<<<<<<< Updated upstream
    if (!ft_strncmp(path, "-", 2))
=======
void f_cd(char **cmd, t_env_list **env)
{
    char *path;
    char *old_pwd;
    int size;

    if (!cmd || !env || !*env)
        return;

    size = size_2d(cmd);
    if (size > 2)
    {
        printf("minishell: cd: too many arguments\n");
        return;
    }

    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
>>>>>>> Stashed changes
    {
        // old = get_path(env, "OLDPWD=", 7) + 7;
        old = getenv("OLDPWD");
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
<<<<<<< Updated upstream
    if (!ft_strncmp(path, "--", 3))
        path = getenv("HOME");
    if (chdir(path) == -1)
    {
        printf("%s file or directory not found!", path);
        exit(1);
=======

    if (size == 1 || !ft_strncmp(cmd[1], "--", 3) || !ft_strncmp(cmd[1], "~", 2))
    {
        path = find_path(*env, "HOME", 4);
        if (!path)
        {
            printf("minishell: cd: HOME not set\n");
            free(old_pwd);
            return;
        }
    }
    else if (ft_strcmp(cmd[1], "-") == 0)
    {
        path = find_path(*env, "OLDPWD", 6);
        if (!path)
        {
            printf("minishell: cd: OLDPWD not set\n");
            free(old_pwd);
            return;
        }
        printf("%s\n", path);
    }
    else
        path = cmd[1];
    if (chdir(path) == -1)
    {
        handle_cd_error(path);
        free(old_pwd);
        return;
>>>>>>> Stashed changes
    }
}

void f_cd(char *path, t_env_list *env)
{
    char *old_pwd;
    char *new_pwd;
    char *current;

    if (!env || !path)
        return ;
    current = getcwd(NULL, 0);
    new_pwd = NULL;
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