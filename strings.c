/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:48:42 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/16 14:10:20 by onajem           ###   ########.fr       */
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

int ft_strlen(const char *s)
{
   int i = 0;

   while (s[i])
      i++;
   return (i);
}

char *join_character(char *s,char c)
{
   char *res;
   int   len;
   int   i;

   i = 0;
   len = ft_strlen(s);
   res = malloc(len + 2);
   while (i < len)
   {
      res[i] = s[i];
      i++;
   }
   res[i] = c;
   res[i + 1] = '\0';
   return (res);
}