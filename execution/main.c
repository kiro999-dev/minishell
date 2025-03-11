#include "../minishell.h"
#include "heper.h"


int main(int ac, char *av[], char *env[])
{
    if (ac > 1 && !ft_strncmp(av[1], "export", 6))
    {
        t_env e;
        e = init_env(env);
        // printf("%d\n", compare_key(e.env_arr, av[2]));
        
        f_export(av + 1, &e);
        print_export(&e);
        free_array(e.env_arr);
        free_array(e.undeclared);
    }

    if (ac > 1 && !ft_strncmp(av[1], "cd", 3))
        f_cd(av[2], env);
    
    if (ac > 1 &&  !ft_strncmp(av[1], "echo", 5))  
        f_echo(av + 1);

    if (ac > 1 && !ft_strncmp(av[1], "pwd", 4))  
        f_env(env);

    }