/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:19:08 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 16:22:13 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	size_2d(char **arr)
{
	int	i;

	if (!arr || !(*arr))
		return (0);
	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	handle_env_i(t_env_list **lst)
{
	t_env_list	*new;
	char		*path;

	path = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	new = new_node(ft_strdup("PWD=/home/onajem/Desktop/minishell"));
	add_back(lst, new);
	new = new_node(ft_strdup("SHLVL=1"));
	add_back(lst, new);
	new = new_node(ft_strdup("_=/usr/bin/env"));
	add_back(lst, new);
	new = new_node(ft_strdup(path));
	add_back(lst, new);
}

char	*handle_shlvl(char *lvl)
{
	char	*equal_sign;
	char	*value_str;
	int		value;
	char	*new_value_str;
	char	*result;

	equal_sign = ft_strchr(lvl, '=');
	if (!equal_sign || !*(equal_sign + 1))
		return (ft_strdup("SHLVL=1"));
	value_str = equal_sign + 1;
	value = ft_atoi(value_str, 0);
	value++;
	if (value > 9998)
	{
		printf("warning: shell level %d too high, resetting to 1\n", value);
		value = 1;
	}
	new_value_str = ft_itoa(value);
	result = ft_strjoin("SHLVL=", new_value_str);
	return (result);
}

t_env_list	*init_env(char *ev[])
{
	int			i;
	t_env_list	*env_list;
	t_env_list	*new;

	i = 0;
	env_list = NULL;
	if (!ev || !ev[0])
		handle_env_i(&env_list);
	else
	{
		while (ev[i])
		{
			if (equal_strcmp(ev[i], "SHLVL") == 0)
				new = new_node(handle_shlvl(ev[i]));
			else
				new = new_node(ft_strdup(ev[i]));
			if (!new)
				(close_fds(), gc_malloc(0, 0), exit(1));
			add_back(&env_list, new);
			i++;
		}
	}
	return (env_list);
}

int	cmds_size(t_exc_lits *lst)
{
	int		i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
