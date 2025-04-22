/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:13:15 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 18:13:16 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	lenght(long long int *n, long long int *len)
{
	*len = 0;
	if (*n <= 0)
	{
		*n *= -1;
		(*len)++;
	}
	while (*n > 0)
	{
		*n /= 10;
		(*len)++;
	}
}

static char	*n_zero(char *dest)
{
	dest[0] = '0';
	return (dest);
}

char	*ft_itoa(int n)
{
	char			*dest;
	long long int	len;
	long long int	temp;

	temp = (long long int)n;
	lenght(&temp, &len);
	dest = gc_malloc(len + 1, 1);
	if (!dest)
		return (NULL);
	dest[len--] = '\0';
	temp = n;
	if (n == 0)
		return (n_zero(dest));
	if (temp < 0)
	{
		dest[0] = '-';
		temp *= -1;
	}
	while (temp > 0)
	{
		dest[len--] = (temp % 10) + '0';
		temp /= 10;
	}
	return (dest);
}
