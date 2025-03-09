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

void f_export(char **cmd, t_env *ev)
{
    // int i;

    if (!cmd)
        exit(1);
    if (size_2d(cmd) == 2)
    {
        sort_env(ev->env_arr);
        f_env(ev->env_arr, 1);
    }
    else
    {
        
    }
        printf("=> %d\n", size_2d(cmd));
    // i = 5;
}



int main(int ac, char *av[], char *env[])
{
    if (ac == 2 && !ft_strncmp(av[1], "export", 6))
    {
        t_env e;
        e = init_env(env);
        f_export(av, &e);
        // f_env(e.env_arr);
    }
}