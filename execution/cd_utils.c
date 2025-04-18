/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:24:31 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 15:25:23 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_path(t_env_list *env, char *key, int len)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (ft_strncmp(env->var, key, len) == 0 && env->var[len] == '=')
			return (ft_strdup(env->var + len + 1));
		env = env->next;
	}
	return (NULL);
}

void	replace_key_value(t_env_list **env, const char *key, const char *value)
{
	int			len;
	t_env_list	*head;
	char		*new_var;

	len = ft_strlen(key);
	head = *env;
	while (head)
	{
		if (ft_strncmp(head->var, key, len) == 0 && head->var[len] == '=')
		{
			new_var = ft_strjoin(key, "=");
			new_var = ft_strjoin(new_var, value);
			if (new_var)
				head->var = new_var;
			return ;
		}
		head = head->next;
	}
	new_var = ft_strjoin(key, "=");
	new_var = ft_strjoin(new_var, value);
	if (new_var)
		add_back(env, new_node(new_var));
}

void	update_pwd(t_data_parsing *data, const char *old_pwd, char *new)
{
	if (!new)
	{
		perror("minishell: cd: getcwd error");
		return ;
	}
	exit_status(0, 1);
	replace_key_value(&data->e, "OLDPWD", old_pwd);
	replace_key_value(&data->e, "PWD", new);
}

void	handle_cd_error(const char *path)
{
	if (access(path, F_OK) == -1)
		write(2, "minishell: cd: No such file or directory\n", 42);
	else if (access(path, X_OK) == -1)
		write(2, "minishell: cd: Permission denied\n", 34);
	else
		write(2, "minishell: cd: Not a directory\n", 32);
	exit_status(1, 1);
}
