#include "../minishell.h"


int main(int ac, char *av[], char *env[])
{
    t_env_list *e;
    e = NULL;
    e = init_env(env);
    if (ac > 1 && !ft_strncmp(av[1], "export", 7))
    {
        f_export(av + 1, e);
        print_export(e);
    }
    if (ac > 1 && !ft_strncmp(av[1], "env", 4))
        f_env(e);

    if (ac > 1 && !ft_strncmp(av[1], "unset", 6))
    {
        f_unset(&e, av);
        print_export(e);
    }

    if (ac > 1 && !ft_strncmp(av[1], "cd", 3))
        f_cd(av[2], env);
    
    if (ac > 1 &&  !ft_strncmp(av[1], "echo", 5))  
        f_echo(av + 1);

    if (ac > 1 && !ft_strncmp(av[1], "pwd", 4))  
        f_pwd();
    }