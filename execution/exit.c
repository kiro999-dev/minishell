#include "../minishell.h"


int is_numeric(const char *str)
{
    int i = 0;

    if (!str)
        return (0);
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!is_num(str[i]))
            return (0);
        i++;
    }
    return (1);
}


void f_exit(char **cmd, t_data_parsing *data_exe)

{
    int exit_status = 0;

    printf("exit\n");
    if (cmd[1])
    {
        if (!is_numeric(cmd[1]))
        {
            printf("minishell: exit: %s: numeric argument required\n", cmd[1]);
            exit_status = 255;
        }
        else if (cmd[2])
        {
            printf("minishell: exit: too many arguments\n");
            return;
        }
        else
            exit_status = ft_atoi(cmd[1]) % 256;
    }
    free_gc(&data_exe->gc_head);
    exit(exit_status);
}