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
    return (NULL);
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

void update_pwd(t_data_parsing *data, const char *old_pwd, char *new)
{
    if (!new)
    {
        perror("minishell: cd: getcwd error");
        return;
    }
    exit_status(0, 1);
    replace_key_value(&data->e, "OLDPWD", old_pwd);
    replace_key_value(&data->e, "PWD", new);
}


void handle_cd_error(const char *path)
{
    if (access(path, F_OK) == -1)
        write(2, "minishell: cd: No such file or directory\n", 42);
    else if (access(path, X_OK) == -1)
        write(2, "minishell: cd: Permission denied\n", 34);
    else
        write(2, "minishell: cd: Not a directory\n", 32);
    exit_status(1, 1);
}


void f_cd(char **cmd, t_data_parsing *data)
{
    char *path;
    char *old_pwd;
    int size;
    char *tmp;

    if (!cmd || !data->e)
        return;

    size = size_2d(cmd);
    if (size > 2)
    {
        write(2, "minishell: cd: too many arguments\n", 35);
        exit_status(1, 1);
        return;
    }
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        char *old_tmp = ft_strdup(data->p_pwd);
        data->p_pwd = ft_strjoin(data->p_pwd, ft_strjoin("/", cmd[1]));
        update_pwd(data, old_tmp, data->p_pwd);
        chdir(cmd[1]);
        tmp = getcwd(NULL, 0);
        if (tmp)
        {
            data->p_pwd = ft_strdup(tmp);
            free(tmp);
        }
        else
            write(2, "cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 109);
        return;
    }
    path = cmd[1];
    if (chdir(path) == -1)
    {
        free(old_pwd);
        handle_cd_error(path);
        return;
    }
    tmp = getcwd(NULL, 0);
    data->p_pwd = ft_strdup(tmp);
    update_pwd(data, old_pwd, data->p_pwd);
    free(old_pwd);
    free(tmp);
}


void f_pwd(t_data_parsing *data)
{

    printf("%s\n", data->p_pwd);
    exit_status(0, 1);

}

void f_env(t_env_list *env, t_exc_lits *cmd)
{
    if (!cmd || !env)
        return ;
    if (cmd->cmd[1] != NULL)
    {
        write(2, "minishell: NO such file or directory\n", 38);
        exit_status(127, 1);
        return ;
    }
    if (!env)
    {
        write(2, "env not found!\n", 16);
        exit_status(127, 1);
        return ;
    }
    while (env)
    {
        if (!env->undec)
            printf("%s\n", env->var);
        env = env->next;
    }
    exit_status(0, 1);
}
