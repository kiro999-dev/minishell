/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:17:18 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/19 21:10:35 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	look_for_c(char *c, char ch)
{
	int	i;

	i = 0;
	while (c[i])
	{
		if (ch == c[i])
			return (1);
		i++;
	}
	return (0);
}

static void	*free_memory(char **res, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}

size_t	counting_words(const char *s1, char *c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s1)
	{
		if (look_for_c(c, *s1))
			in_word = 0;
		else
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		s1++;
	}
	return (count);
}

static char	*fill(char *s, char *c, int *ptr_i)
{
	size_t	word_len;
	char	*str;
	int		start;

	while (s[*ptr_i] && look_for_c(c, s[*ptr_i]))
		(*ptr_i)++;
	start = *ptr_i;
	while (s[*ptr_i] && !look_for_c(c, s[*ptr_i]))
		(*ptr_i)++;
	word_len = *ptr_i - start;
	str = (char *)gc_malloc(word_len + 1,1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s + start, word_len + 1);
	return (str);
}

char	**ft_split(const char *s, char *c)
{
	char	**res;
	int		k;
	int		i;

	if (!s)
		return (NULL);
	res = (char **)gc_malloc(sizeof(char *) * (counting_words(s, c) + 1),1);
	if (!res)
		return (NULL);
	i = 0;
	k = 0;
	while (s[i])
	{
		if (!look_for_c(c, s[i]))
		{
			res[k] = fill((char *)s, c, &i);
			if (!res[k])
				return (free_memory(res, k));
			k++;
		}
		else
			i++;
	}
	res[k] = NULL;
	return (res);
}