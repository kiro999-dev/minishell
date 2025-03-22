#include "../minishell.h"

char	*prepare_path(char *path, char *cmd)
{
	char	**tmp;
	char	*dest;
	char	*temp;
	int		i;

	tmp = ft_split(path, ":");
	if (!tmp)
		exit(1);
	i = 0;
	while (tmp[i])
	{
		temp = ft_strjoin(tmp[i], "/");
		dest = ft_strjoin(temp, cmd);
		/* Depending on your memory management you might want to free(temp) here */
		if (access(dest, X_OK) == 0)
			return (dest);
		i++;
	}
	exit(1);
}

char	*get_path(t_env_list *env, char *cmd)
{
	if (!env || !cmd)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (cmd);
	else if (access(cmd, X_OK) == -1 &&
		(ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "/", 1) == 0))
		return (NULL);
	while (env)
	{
		if (!ft_strncmp(env->var, "PATH=", 5))
			break;
		env = env->next;
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
	return (!ft_strncmp(cmd, "export", 7) || !ft_strncmp(cmd, "env", 4) ||
			!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "cd", 3) ||
			!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "pwd", 4));
}

void	exec_builtin(t_exc_lits *cmd, t_data_parsing *data_exec)
{
	if (!ft_strncmp(cmd->cmd[0], "export", 7))
		f_export(cmd->cmd, &data_exec->e);
	else if (!ft_strncmp(cmd->cmd[0], "env", 4))
		f_env(data_exec->e);
	else if (!ft_strncmp(cmd->cmd[0], "unset", 6))
		f_unset(&data_exec->e, cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "cd", 3))
		f_cd(cmd->cmd, data_exec->e);
	else if (!ft_strncmp(cmd->cmd[0], "echo", 5))
		f_echo(cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "pwd", 4))
		f_pwd();
}

// void	handle_redirection(t_data_parsing *data)
// {
// 	if (!data->head_file)
// 	{
// 		printf("nofile\n");
// 		return ;
// 	}
// 	printf("here\n");
// 	while (data->head_file)
// 	{
// 		printf("%s [%u]\n", data->head_file->file, data->head_file->type);
// 		data->head_file = data->head_file->next;
// 	}
// }

/*
 * Revised execution() function to support pipelines.
 * It uses t_exc_lits for the command list and assumes that:
 * - cmds_size() returns the number of commands in data_exec->head_exe.
 * - data_exec->head_file holds redirection info (if any).
 */
void	execution(t_data_parsing *data_exec)
{
	int			input_fd = 0;    // Initially standard input (fd 0)
	t_exc_lits	*cmd_lst = data_exec->head_exe;
	int			fd[2];
	pid_t		pid;
	int			status;

	if (!cmd_lst)
		return;

	/* Handle any redirections before launching commands */
	// handle_redirection(data_exec);

	/* If there is only one command, run it without setting up a pipeline */
	if (cmds_size(data_exec->head_exe) == 1)
	{
		if (is_builtin(cmd_lst->cmd[0]))
		{
			exec_builtin(cmd_lst, data_exec);
			return;
		}
		pid = fork();
		if (pid == 0)
		{
			char *path = get_path(data_exec->e, cmd_lst->cmd[0]);
			char **env = env_list_to_array(data_exec->e);
			if (!path || !env)
			{
				printf("Command not found or environment error!\n");
				exit(EXIT_FAILURE);
			}
			execve(path, cmd_lst->cmd, env);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		waitpid(pid, &status, 0);
		return;
	}

	/* Loop through all commands in the pipeline */
	while (cmd_lst)
	{
		/* If there is a next command, create a pipe */
		if (cmd_lst->next)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				return;
			}
		}

		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			return;
		}
		if (pid == 0)
		{
			/* Child process */
			if (input_fd != 0)
			{
				if (dup2(input_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 input");
					exit(EXIT_FAILURE);
				}
				close(input_fd);
			}
			if (cmd_lst->next)
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2 output");
					exit(EXIT_FAILURE);
				}
				close(fd[0]);
				close(fd[1]);
			}
			if (is_builtin(cmd_lst->cmd[0]))
			{
				exec_builtin(cmd_lst, data_exec);
				exit(EXIT_SUCCESS);
			}
			else
			{
				char *path = get_path(data_exec->e, cmd_lst->cmd[0]);
				char **env = env_list_to_array(data_exec->e);
				if (!path || !env)
				{
					printf("Command not found or environment error!\n");
					exit(EXIT_FAILURE);
				}
				execve(path, cmd_lst->cmd, env);
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		/* Parent process */
		if (input_fd != 0)
			close(input_fd);
		if (cmd_lst->next)
		{
			close(fd[1]);
			input_fd = fd[0];
		}
		cmd_lst = cmd_lst->next;
	}
	/* Wait for all child processes to finish */
	while (wait(NULL) > 0);
}
