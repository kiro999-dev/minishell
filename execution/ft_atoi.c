/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:12:18 by onajem            #+#    #+#             */
/*   Updated: 2025/04/20 18:09:15 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	is_num(char c)
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

int	ft_atoi(const char *nptr, int ex)
{
	int			sign;
	long long	sum;
	const char	*tmp;

	sum = 0;
	tmp = nptr;
	if (!ft_strcmp((char *)nptr, "-9223372036854775808") && ex)
		(close_fds(), gc_malloc(0, 0), exit(0));
	sign = num_start(&nptr);
	while (is_num(*nptr))
	{
		if (check_overflow(sum, *nptr, sign) != 1)
		{
			if (ex == 0)
				return (0);
			print_error(tmp, ": numeric argument required\n", " exit: ");
			(close_fds(), gc_malloc(0, 0), exit(2));
		}
		sum = sum * 10 + (*nptr - '0');
		nptr++;
	}
	return (sum * sign);
}
