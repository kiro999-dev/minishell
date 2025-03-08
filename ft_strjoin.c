/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:23:40 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/07 17:49:27 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	len_res;

	if (!s1 || !s2)
		return (0);
	len_res = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = malloc(len_res);
	if (!res)
		return (NULL);
	res[0] = '\0';
	ft_strlcat(res, (char *)s1, len_res);
	ft_strlcat(res, (char *)s2, len_res);
	return (res);
}
