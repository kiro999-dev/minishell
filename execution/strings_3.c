/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:27:58 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 18:28:23 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

int	ft_isalnum(int ch)
{
	if ((ch >= 48 && ch <= 57) || ((ch >= 65 && ch <= 90)
			|| (ch >= 97 && ch <= 122)))
		return (1);
	return (0);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (dst == src)
		return (dst);
	if (!dst && !src)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return ((void *)dst);
}

void	hexa_format(unsigned int value, char *output)
{
	const char	*hex_digits;
	int			i;

	i = 0;
	hex_digits = "0123456789abcdef";
	while (i < 8)
	{
		output[i] = hex_digits[(value >> (28 - i * 4)) & 0xF];
		i++;
	}
	output[8] = '\0';
}
