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


void ft_putstr(char *str)
{
    int i = 0;
	printf("declare -x ");
    while (str[i])
    {
        printf("%c", str[i]);
		if (str[i] == '=' || !str[i + 1])
			printf("\"");
        i++;
    }
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
	dest = (char *)malloc(len_s1 + len_s2 + 1);
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
	while ((s1[i] || s2[i]) && s1[i] != '=')
	{
		if (s1[i] != s2[i])
            return (1);
		i++;
	}
	return (0);
}