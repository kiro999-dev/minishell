#include "../minishell.h"


// char	*ft_strdup(const char *s1)
// {
// 	char	*dest;
// 	size_t	i;

// 	dest = (char *)malloc(ft_strlen(s1) + 1);
// 	if (!dest)
// 		return (NULL);
// 	i = 0;
// 	while (s1[i])
// 	{
// 		dest[i] = s1[i];
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }



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

static void	set_prefix(char *dst, char const *src, size_t i, size_t s_len)
{
	while (i < s_len)
	{
		dst[i] = src[i];
		i++;
	}
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*dest;
	size_t	len_s1;
	size_t	len_s2;
	size_t	i;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return (ft_strdup(s2));
	if (!s2 && s1)
		return (ft_strdup(s1));
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	dest = (char *)gc_malloc(len_s1 + len_s2 + 1);
	if (!dest)
		return (NULL);
	i = 0;
	set_prefix(dest, s1, i, len_s1);
	while (i < len_s2)
	{
		dest[i + len_s1] = s2[i];
		i++;
	}
	dest[len_s1 + len_s2] = '\0';
	return (dest);
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

char	*ft_substr(char const *s, int start, int len)
{
	char	*dest;
	int	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	dest = (char *)gc_malloc(len + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < len && s[start])
	{
		dest[i] = s[start];
		i++;
		start++;
	}
	dest[len] = '\0';
	return (dest);
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	dest_len;
	size_t	src_len;

	if (!dest && size == 0)
		return (ft_strlen(src));
	src_len = ft_strlen(src);
	dest_len = ft_strlen(dest);
	i = 0;
	if (dest_len >= size)
	{
		return (src_len + size);
	}
	while (dest_len + i < size - 1 && src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (src_len + dest_len);
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