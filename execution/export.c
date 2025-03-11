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
            printf("declare -x %s\n", e->undeclared[i]);
    
    }
}

void add_var_2_env(char *cmd, t_env *e)
{
    char **new_env;
    int i;

    new_env = malloc(sizeof(char *) * (e->size + 2));
    if (!new_env)
        return ;
    new_env[e->size] = ft_strdup(cmd);
    new_env[e->size + 1] = NULL;
    i = -1;
    while (++i < e->size)
        new_env[i] = e->env_arr[i];
    free(e->env_arr);
    e->size++;
    e->env_arr = new_env;
}



void add_var_2_undeclared(char *cmd, t_env *e)
{
    char **new_undeclared;
    int i;
// didnt work try use unset to fix this   ;   peace out broo//
    if (compare_key(e->env_arr, cmd))
        return ;
    new_undeclared = malloc(sizeof(char *) * (e->size_undec + 2));
    if (!new_undeclared)
        return ;
    new_undeclared[e->size_undec] = ft_strdup(cmd);
    new_undeclared[e->size_undec + 1] = NULL;
    i = -1;
    while (++i < e->size_undec)
        new_undeclared[i] = e->undeclared[i];
    free(e->undeclared);
    e->size_undec++;
    e->undeclared = new_undeclared;
}

void f_export(char **cmd, t_env *ev)
{
    int i;

    if (!cmd)
        exit(1);
    if (size_2d(cmd) == 1)
    {
        sort_env(ev->env_arr);
        print_export(ev);
    }
    else
    {
        i = 1;
        while (cmd[i])
        {
            if (!ft_strchr(cmd[i], '='))
                add_var_2_undeclared(cmd[i], ev);
            else
                add_var_2_env(cmd[i], ev);
            i++;    
        }
    }
}
