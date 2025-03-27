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


void export_putstr(char *str)
{
    int i;
	short first_equal;

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