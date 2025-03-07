#include "../minishell.h"

int echo_flag(char *str)
{
    int i;

    i = 0;
    if (str[i] == '-' && str[i + 1])
        i++;
    else 
        return (0);
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

// void ft_putstr(char *str)
// {
//     int i = 0;

//     while (str[i])
//     {
//         write(1, str + i, 1);
//         i++;
//     }
// }

void f_echo(char **av)
{
    int i;
    int nwl;

    i = 2;
    nwl = 0;
    while (av[i] && echo_flag(av[i]))
    {
        nwl = 1;
        i++;
    }
    while (av[i])
    {
        printf("%s", av[i]);
        if (av[i + 1])
            printf(" ");
        i++;
    }
    if (nwl == 0 || !av[2])
        printf("\n");
}

// int main(int ac, char *av[])
// {
//     if (ac > 1)
//     {
//         if (!ft_strncmp(av[1], "echo", 5))
//             f_echo(av);
//         else 
//             printf("command not Xfound!");
//     }
// }