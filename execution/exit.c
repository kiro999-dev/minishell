#include "heper.h"
#include "../minishell.h"

static int	check_overflow(long long num, char digit, int is_neg)
{
	if (num > LLONG_MAX / 10
		|| (num == LLONG_MAX / 10 && (digit - '0') > LLONG_MAX % 10))
	{
		if (is_neg == -1)
			return (0);
		else if (is_neg == 1)
			return (-1);
	}
	return (1);
}

static int	is_num(char c)
{
	return (c >= '0' && c <= '9');
}

static int	num_start(const char **str)
{
	int	sign;

	sign = 1;
	while (**str == ' ' || (**str >= 9 && **str <= 13))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign *= -1;
		(*str)++;
	}
	return (sign);
}

int	ft_atoi(const char *nptr)
{
	int			sign;
	long long	sum;

	sum = 0;
	sign = num_start(&nptr);
	while (is_num(*nptr))
	{
		if (check_overflow(sum, *nptr, sign) != 1)
			return (check_overflow(sum, *nptr, sign));
		sum = sum * 10 + (*nptr - '0');
		nptr++;
	}
	return (sum * sign);
}

// void f_exit(char **cmd, t_env_list *env, t_garbage_collector *gc)
void f_exit(char **cmd, t_env_list *env)

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
    // free_gc(env, gc);
    exit(exit_status);
}