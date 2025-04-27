/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:58:31 by onajem            #+#    #+#             */
/*   Updated: 2025/04/19 14:53:27 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_append(t_env_list *env, char *var)
{
	char	*key;
	char	*value;
	char	*tmp;

	key = NULL;
	value = NULL;
	get_key_value(var, &key, &value);
	if (!key || !value)
		return (0);
	while (env)
	{
		if (!equal_strcmp(key, env->var))
		{
			if (!ft_strchr(env->var, '='))
				value = ft_strjoin("=", value);
			tmp = ft_strjoin(env->var, value);
			env->var = tmp;
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	add_var_2_env(char *cmd, t_env_list **env)
{
	t_env_list	*new;
	int			if_key_exist;

	if (check_append(*env, cmd))
		return ;
	if_key_exist = replace_existing_key(*env, cmd);
	if (if_key_exist == 1)
		return ;
	else if (if_key_exist == -1)
		cmd = trim_plus_sign(cmd);
	new = new_node(cmd);
	if (!new)
		return ;
	if (!ft_strchr(cmd, '='))
		new->undec = 1;
	add_back(env, new);
}

t_env_list	*copy_list(t_env_list *env)
{
	t_env_list	*new_list;
	t_env_list	*current;
	t_env_list	*new;

	new_list = NULL;
	current = env;
	while (current)
	{
		new = new_node(ft_strdup(current->var));
		if (!new)
			return (NULL);
		add_back(&new_list, new);
		current = current->next;
	}
	return (new_list);
}

void	handle_unvalid_key(char *cmd, t_env_list **env)
{
	char	**splited_value;
	char	*tmp;
	int		i;

	tmp = ft_strchr(cmd, '=');
	if (!tmp)
	{
		print_error(cmd, ": not a valid identifier\n", " export");
		exit_status(1, 1);
		return ;
	}
	splited_value = ft_split(tmp + 1, " ");
	if (!splited_value)
		return ;
	i = 1;
	if (ft_isspace(*(tmp + 1)) == 1)
		i--;
	while (splited_value[i])
	{
		add_var_2_env(splited_value[i], env);
		i++;
	}
	print_error(splited_value[1], ": not a valid identifier\n", " export");
	exit_status(1, 1);
}

void	f_export(char **cmd, t_env_list **ev, int i)
{
	t_env_list	*tmp;

	if (!cmd)
		(close_fds(), gc_malloc(0, 0), exit(1));
	exit_status(0, 1);
	if (size_2d(cmd) == 1)
	{
		tmp = copy_list(*ev);
		if (!tmp)
			return ;
		sort_env(&tmp);
		print_export(tmp);
	}
	else
	{
		while (cmd[i])
		{
			if (!valid_key(cmd[i]))
				handle_unvalid_key(cmd[i], ev);
			else
				add_var_2_env(cmd[i], ev);
			i++;
		}
	}
}
