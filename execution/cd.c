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
    replace_key_value(&data->e, "OLDPWD", old_pwd);
    replace_key_value(&data->e, "PWD", new);

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




// leaks need to be heandled with getcwd asap !!!!!!!
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
        printf("minishell: cd: too many arguments\n");
        return;
    }
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        char *old_tmp = ft_strdup(data->p_pwd);
        data->p_pwd = ft_strjoin(data->p_pwd, ft_strjoin("/", cmd[1]));
        update_pwd(data, old_tmp, data->p_pwd);
        chdir(cmd[1]);
        printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
        return;
    }
    path = cmd[1];
    if (chdir(path) == -1)
    {
        free(old_pwd);
        handle_cd_error(path);
        return;
    }
    update_pwd(data, old_pwd, data->p_pwd);
    tmp = getcwd(NULL, 0);
    data->p_pwd = ft_strdup(tmp);
    free(old_pwd);
    free(tmp);
}


void f_pwd(t_data_parsing *data)
{

    printf("%s\n", data->p_pwd);

}

void f_env(t_env_list *env, t_exc_lits *cmd)
{
    if (!cmd || !env) 
        return ;
    if (cmd->cmd[1] != NULL)
    {
        printf("minishell: '%s': NO such file or directory\n", cmd->cmd[1]);
        return ;
    }
    if (!env)
    {
        printf("env not found!\n");
        return ;
    }
    while (env)
    {
        if (!env->undec)
            printf("%s\n", env->var);
        env = env->next;
    }
}
