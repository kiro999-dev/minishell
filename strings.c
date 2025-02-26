/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:48:42 by zkhourba          #+#    #+#             */
/*   Updated: 2025/02/26 13:45:45 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len_src;
	size_t	i;

	i = 0;
	len_src = ft_strlen(src);
	if (dstsize == 0)
		return (len_src);
	while (i < dstsize - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len_src);
}

char	*ft_strdup(const char *s1)
{
	char		*s1_cpy;
	size_t		size;

	size = ft_strlen(s1) + 1;
	s1_cpy = malloc(size);
	if (!s1_cpy)
		return (NULL);
	ft_strlcpy(s1_cpy, s1, size);
	return (s1_cpy);
}
char	*ft_strchr(const char *s, int c)
{
	char	*ptr_s;

	ptr_s = (char *) s;
	while (*ptr_s)
	{
		if (*ptr_s == (char) c)
			return (ptr_s);
		ptr_s++;
	}
	if (*ptr_s == (char)c)
		return (ptr_s);
	return (NULL);
}