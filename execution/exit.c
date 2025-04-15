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


void f_exit(char **cmd, t_data_parsing *data_exe, int child)
{
    int exit_status;
    
    exit_status = 0;
    if (child != 1)
        printf("exit\n");
    if (cmd[1])
    {
        if (!is_numeric(cmd[1]))
        {
            write(2, "minishell : exit : numeric argument required\n", 46);
            exit_status = 255;
        }
        else if (cmd[2])
        {
            write(2, "minishell : exit : too many arguments\n", 39);
            return;
        }
        else
        exit_status = ft_atoi(cmd[1]) % 256;
    }
    free_gc(&data_exe->gc_head);
    exit(exit_status);
}
