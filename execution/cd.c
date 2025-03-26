#include "../minishell.h"

char *find_path(t_env_list *env, char *key, int len)
{
    if (!env || !key)
        return NULL;

    while (env)
    {
        if (ft_strncmp(env->var, key, len) == 0 && env->var[len] == '=')
            return ft_strdup(env->var + len + 1);
        env = env->next;
    }
    return NULL;
}


void replace_key_value(t_env_list **env, const char *key, const char *value)
{
    int len = ft_strlen(key);
    t_env_list *head = *env;
    char *new_var;

    while (head)
    {
        if (ft_strncmp(head->var, key, len) == 0 && head->var[len] == '=')
        {
            new_var = ft_strjoin(key, "=");
            new_var = ft_strjoin(new_var, value);
            if (new_var)
                head->var = new_var;
            return;
        }
        head = head->next;
    }

    new_var = ft_strjoin(key, "=");
    new_var = ft_strjoin(new_var, value);
    if (new_var)
        add_back(env, new_node(new_var));
}

void update_pwd(t_env_list **env, const char *old_pwd)
{
    char *current_pwd = getcwd(NULL, 0);
    if (!current_pwd)
    {
        perror("minishell: cd: getcwd error");
        return;
    }

    replace_key_value(env, "OLDPWD", old_pwd);
    replace_key_value(env, "PWD", current_pwd);
}


void handle_cd_error(const char *path)
{
    if (access(path, F_OK) == -1)
        printf("minishell: cd: %s: No such file or directory\n", path);
    else if (access(path, X_OK) == -1)
        printf("minishell: cd: %s: Permission denied\n", path);
    else
        printf("minishell: cd: %s: Not a directory\n", path);
}

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
    {
        perror("minishell: cd");
        return;
    }

    if (size == 1 || !ft_strncmp(cmd[1], "--", 3) || !ft_strncmp(cmd[1], "~", 2))
    {
        path = find_path(*env, "HOME", 4);
        if (!path)
        {
            printf("minishell: cd: HOME not set\n");
            return;
        }
    }
    else if (ft_strcmp(cmd[1], "-") == 0)
    {
        path = find_path(*env, "OLDPWD", 6);
        if (!path)
        {
            printf("minishell: cd: OLDPWD not set\n");
            return;
        }
        printf("%s\n", path);
    }
    else
        path = cmd[1];
    if (chdir(path) == -1)
    {
        handle_cd_error(path);
        return;
    }
    update_pwd(env, old_pwd);
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