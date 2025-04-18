/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:28:51 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 18:29:29 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		check;

	i = 0;
	check = 0;
	while ((s1[i] || s2[i]) && (i < n))
	{
		if (s1[i] != s2[i])
		{
			check = (unsigned char)s1[i] - (unsigned char)s2[i];
			break ;
		}
		i++;
	}
	return (check);
}

void	export_putstr(char *str)
{
	int	i;
	int	first_equal;

	i = 0;
	first_equal = 0;
	printf("declare -x ");
	while (str[i])
	{
		printf("%c", str[i]);
		if (str[i] == '=' && !first_equal)
		{
			first_equal = 1;
			printf("\"");
		}
		if (!str[i + 1] && first_equal)
			printf("\"");
		i++;
	}
	printf("\n");
}

int	equal_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] != '=' && s2[i] && s2[i] != '=')
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	if ((s1[i] == '=' || !s1[i]) && (s2[i] == '=' || !s2[i]))
		return (0);
	return (1);
}

static int	str_cmp(const char *s1, const char *s2, size_t j, size_t len)
{
	size_t	i;

	i = 0;
	while (s2[i] && s1[i] == s2[i] && i + j < len)
	{
		i++;
	}
	if ((size_t)ft_strlen(s2) == i)
		return (0);
	return (1);
}

char	*custom_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;

	i = 0;
	if (needle && len == 0 && !haystack)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	if (ft_strlen(needle) > ft_strlen(haystack))
		return (NULL);
	while (haystack[i] && i < len && haystack[i] != '=')
	{
		if (str_cmp(haystack + i, needle, i, len) == 0)
		{
			return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}
