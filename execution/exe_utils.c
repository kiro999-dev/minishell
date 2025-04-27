/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:34:32 by onajem            #+#    #+#             */
/*   Updated: 2025/04/21 18:28:51 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*prepare_path(char *path, char *cmd)
{
	char	**tmp;
	char	*dest;
	char	*temp;
	int		i;

	if (!path || !cmd)
		return (NULL);
	tmp = ft_split(path, ":");
	if (!tmp)
		return (NULL);
	i = -1;
	while (tmp[++i])
	{
		temp = ft_strjoin(tmp[i], "/");
		dest = ft_strjoin(temp, cmd);
		if (access(dest, F_OK) == 0)
		{
			if (access(dest, X_OK) == 0)
				return (dest);
			write(2, "minishell: Permission denied\n", 30);
			return (NULL);
		}
	}
	write(2, "minishell: command not found\n", 30);
	(close_fds(), exit(127));
}

char	*get_path(t_env_list *env, char *cmd)
{
	if (!env || !cmd)
		return (NULL);
	if (ft_strlen(cmd) == 0)
	{
		write(2, "minishell: command not found\n", 30);
		(close_fds(), exit(127));
	}
	if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
		return (cmd);
	while (env)
	{
		if (!ft_strncmp(env->var, "PATH=", 5))
			break ;
		env = env->next;
	}
	if (!env)
	{
		if (access(ft_strjoin("./", cmd), X_OK) == 0)
			return (cmd);
		write(2, "minishell: No such file or directory\n", 38);
		(close_fds(), exit(127));
	}
	return (prepare_path(env->var + 5, cmd));
}

char	**env_list_to_array(t_env_list *list)
{
	int		count;
	int		i;
	char	**env;

	count = env_size(list);
	env = (char **)gc_malloc(sizeof(char *) * (count + 1), 1);
	if (!env)
		return (NULL);
	env[count] = NULL;
	i = 0;
	while (i < count)
	{
		env[i] = ft_strdup(list->var);
		if (!env[i])
		{
			gc_malloc(0, 0);
			return (NULL);
		}
		list = list->next;
		i++;
	}
	return (env);
}

int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	return (!ft_strncmp(cmd, "export", 7) || !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "pwd", 4)
		|| !ft_strncmp(cmd, "exit", 5));
}

void	exec_builtin(t_exc_lits *cmd, t_data_parsing *data_exec, int child)
{
	if (!ft_strncmp(cmd->cmd[0], "export", 7))
		f_export(cmd->cmd, &data_exec->e, 1);
	else if (!ft_strncmp(cmd->cmd[0], "env", 4))
		f_env(data_exec->e, cmd);
	else if (!ft_strncmp(cmd->cmd[0], "unset", 6))
		f_unset(&data_exec->e, cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "cd", 3))
		f_cd(cmd->cmd, data_exec);
	else if (!ft_strncmp(cmd->cmd[0], "echo", 5))
		f_echo(cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "pwd", 4))
		f_pwd(data_exec);
	else if (!ft_strncmp(cmd->cmd[0], "exit", 5))
		f_exit(cmd->cmd, child);
}
