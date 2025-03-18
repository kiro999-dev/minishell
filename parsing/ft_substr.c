/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:02:01 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 17:22:24 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(char const *s, int start, int len)
{
	char	*sub_str;
	int	i;

	sub_str = NULL;
	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup("\0"));
	if (len > ft_strlen(s))
		sub_str = (char *)gc_malloc(ft_strlen(s + start) + 1);
	else if (ft_strlen(s) - start < len)
		sub_str = gc_malloc(ft_strlen(s) - start + 1);
	else
		sub_str = (char *) gc_malloc(len + 1);
	if (!sub_str)
		return (NULL);
	while (i < len && s[start])
	{
		sub_str[i] = s[start];
		i++;
		start++;
	}
	sub_str[i] = '\0';
	return (sub_str);
}
