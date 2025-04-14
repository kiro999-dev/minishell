#include "../minishell.h"

void echo_putstr(char *s)
{
    size_t i;

    if (!s)
        return;
    i = 0;
    while (s[i])
    {
        write(1, s + i, 1);
        i++;
    }
}

int echo_flag(char *str)
{
    int i;

    if (!str || str[0] != '-')
        return (0);
    
    i = 1;
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

void f_echo(char **cmd)
{
    int i;
    int nwl;
    int has_content;

    i = 1;
    nwl = 0;
    has_content = 0;
    
    while (cmd[i] && echo_flag(cmd[i]))
    {
        nwl = 1;
        i++;
    }

    while (cmd[i])
    {
        if (has_content)
            write(1, " ", 1);
        echo_putstr(cmd[i]);
        has_content = 1;
        i++;
    }

    if (!nwl)
        write(1, "\n", 1);
}