/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:43:42 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 17:57:58 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_env(t_env_list **head)
{
	t_env_list	*ptr;
	t_env_list	*last;
	int			swapped;
	char		*temp;

	last = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr = *head;
		while (ptr->next != last)
		{
			if (ft_strncmp(ptr->var, ptr->next->var, ft_strlen(ptr->var)) > 0)
			{
				temp = ptr->var;
				ptr->var = ptr->next->var;
				ptr->next->var = temp;
				swapped = 1;
			}
			ptr = ptr->next;
		}
		last = ptr;
	}
}

char	*trim_plus_sign(char *key)
{
	char	*equal_pos;
	char	*new_key;
	int		len_before_plus;
	int		total_len;

	equal_pos = custom_strnstr(key, "+=", ft_strlen(key));
	if (!equal_pos)
		return (key);
	len_before_plus = equal_pos - key;
	total_len = ft_strlen(key) - 1;
	new_key = gc_malloc(total_len + 1, 1);
	if (!new_key)
		return (NULL);
	ft_memcpy(new_key, key, len_before_plus);
	ft_memcpy(new_key + len_before_plus, equal_pos + 1, ft_strlen(equal_pos));
	new_key[total_len] = '\0';
	return (new_key);
}

int	replace_existing_key(t_env_list *env, char *key)
{
	t_env_list	*current;
	char		*declared;

	current = env;
	if (custom_strnstr(key, "+=", ft_strlen(key)))
		return (-1);
	declared = ft_strchr(key, '=');
	while (current)
	{
		if (declared && equal_strcmp(current->var, key) == 0)
		{
			current->undec = 0;
			current->var = ft_strdup(key);
			return (1);
		}
		if (!declared && equal_strcmp(current->var, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

int	valid_key(char *var)
{
	int		i;
	int		key_len;
	char	*equal;

	if (!var || !var[0] || (!(ft_isalpha(var[0]) || var[0] == '_')))
		return (0);
	equal = ft_strchr(var, '=');
	if (equal)
		key_len = equal - var;
	else
		key_len = ft_strlen(var);
	if (key_len > 1 && var[key_len - 1] == '+')
		key_len--;
	i = 1;
	while (i < key_len)
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	get_key_value(char *var, char **key, char **value)
{
	char	*plus_pos;

	plus_pos = custom_strnstr(var, "+=", ft_strlen(var));
	if (!plus_pos)
		return ;
	*key = ft_substr(var, 0, plus_pos - var);
	if (!(*key))
		return ;
	*value = ft_strdup(plus_pos + 2);
	if (!(*value))
	{
		return ;
	}
}
