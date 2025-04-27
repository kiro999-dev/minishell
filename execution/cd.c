/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:48:15 by onajem            #+#    #+#             */
/*   Updated: 2025/04/19 15:10:18 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_getcwd_fails(t_data_parsing *data, char *cmd)
{
	char	*tmp;
	char	*old_tmp;

	old_tmp = ft_strdup(data->p_pwd);
	data->p_pwd = ft_strjoin(data->p_pwd, ft_strjoin("/", cmd));
	update_pwd(data, old_tmp, data->p_pwd);
	chdir(cmd);
	tmp = getcwd(NULL, 0);
	if (tmp)
	{
		data->p_pwd = ft_strdup(tmp);
		free(tmp);
	}
	else
	{
		write(2, "cd: error retrieving current directory: getcwd: cannot", 55);
		write(2, " access parent directories: No such file or directory\n", 55);
	}
}

void	update_paths(t_data_parsing *data, char *old_pwd, char *path)
{
	char	*tmp;

	if (!old_pwd)
	{
		handle_getcwd_fails(data, path);
		return ;
	}
	if (chdir(path) == -1)
	{
		free(old_pwd);
		handle_file_error(path, 1);
		return ;
	}
	tmp = getcwd(NULL, 0);
	data->p_pwd = ft_strdup(tmp);
	update_pwd(data, old_pwd, data->p_pwd);
	free(old_pwd);
	free(tmp);
}

void	f_cd(char **cmd, t_data_parsing *data)
{
	char	*old_pwd;
	int		size;
	char	*path;

	if (!cmd || !data->e)
		return ;
	size = size_2d(cmd);
	if (size > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		exit_status(1, 1);
		return ;
	}
	old_pwd = getcwd(NULL, 0);
	path = cmd[1];
	if (size == 1)
	{
		path = find_path(data->e, "HOME", 4);
		if (!path)
		{
			(free(old_pwd), write(2, "minishell: cd: HOME not set\n", 29));
			exit_status(1, 1);
			return ;
		}
	}
	update_paths(data, old_pwd, path);
}

void	f_pwd(t_data_parsing *data)
{
	printf("%s\n", data->p_pwd);
	exit_status(0, 1);
}

void	f_env(t_env_list *env, t_exc_lits *cmd)
{
	if (!cmd || !env)
		return ;
	if (cmd->cmd[1] != NULL)
	{
		print_error(cmd->cmd[1], ": NO such file or directory\n", NULL);
		exit_status(127, 1);
		return ;
	}
	if (!env)
	{
		write(2, "env not found!\n", 16);
		exit_status(127, 1);
		return ;
	}
	while (env)
	{
		if (!env->undec)
			printf("%s\n", env->var);
		env = env->next;
	}
	exit_status(0, 1);
}
