/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:23:40 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/17 03:30:45 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcat(char *dest, char *src, unsigned int size)
{
	unsigned int	j;
	unsigned int	len_src;
	unsigned int	len_dst;
	unsigned int	n;

	j = 0;
	n = 0;
	len_src = ft_strlen(src);
	if (size == 0)
	{
		return (len_src);
	}
	len_dst = ft_strlen(dest);
	if (len_dst >= size)
		return (len_src + size);
	while (dest[j] && j < size - 1)
		j++;
	while ((j < size - 1) && src[n])
	{
		dest[j] = src[n];
		n++;
		j++;
	}
	dest[j] = '\0';
	return (len_src + len_dst);
}
char	*ft_strjoin(char const *s1, char const *s2,t_gc_collector **gc_head)
{
	char	*res;
	size_t	len_res;

	if (!s1 && s2)
		return(ft_strdup(s2,gc_head));
	else if(!s2 && s1)
		return(ft_strdup(s1,gc_head));
	else if(!s2 && !s1)
		return(NULL);
	len_res = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = gc_malloc(gc_head,len_res);
	if (!res)
		return (NULL);
	res[0] = '\0';
	ft_strlcat(res, (char *)s1, len_res);
	ft_strlcat(res, (char *)s2, len_res);
	return (res);
}
