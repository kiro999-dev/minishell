#include "../minishell.h"
#include "heper.h"

void sort_env(char **env) {
    int i, j;
    char *temp;
    
    for (i = 0; env[i]; i++) {
        for (j = i + 1; env[j]; j++) {
            if (strcmp(env[i], env[j]) > 0) {
                temp = env[i];
                env[i] = env[j];
                env[j] = temp;
            }
        }
    }
}

void print_export(t_env *e)
{
    int i;

    if (!e->env_arr)
    {
        printf("varibales not found!\n");
        return ;
    }
    i = -1;
    while (e->env_arr[++i])
    {
        ft_putstr(e->env_arr[i]);
        printf("\n");
    }
    i = -1;
    if (e->undeclared)
    {
        while (e->undeclared[++i])
        {
            printf("declare -x %s\n", e->undeclared[i]);
            printf("\n");
        }
    }
}

void add_var_2_env(char **cmd, t_env *e)
{
    char **new_env;
    int i;

    new_env = malloc(sizeof(char *) * (e->size + 2));
    if (!new_env)
        return ;
    new_env[e->size] = ft_strdup(cmd[2]);
    new_env[e->size + 1] = NULL;
    i = -1;
    while (++i < e->size)
        new_env[i] = e->env_arr[i];
    free_array(e->env_arr);
    e->size++;
    e->env_arr = new_env;
}

void add_var_2_undeclared(char **cmd, t_env *e)
{
    char **new_undeclared;
    int i;

    e->size_undec = size_2d(e->undeclared);
    new_undeclared = malloc(sizeof(char *) * (e->size_undec + 2));
    if (!new_undeclared)
        return ;
    new_undeclared[e->size_undec] = ft_strdup(cmd[2]);
    new_undeclared[e->size_undec + 1] = NULL;
    i = -1;
    while (++i < e->size_undec)
        new_undeclared[i] = e->undeclared[i];
    free_array(e->undeclared);
    e->size_undec++;
    e->undeclared = new_undeclared;
}

void f_export(char **cmd, t_env *ev)
{
    // int i;

    if (!cmd)
        exit(1);
    if (size_2d(cmd) == 2)
    {
        sort_env(ev->env_arr);
        print_export(ev);
    }
    else
    {
        printf("here\n");
        if (ft_strchr(cmd[2], '='))
            add_var_2_env(cmd, ev);
        else
            add_var_2_undeclared(cmd, ev);
    }
}
