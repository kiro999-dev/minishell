#include "../minishell.h"





char	*prepare_path(char *path, char *cmd)
{
    char	**tmp;
	char	*dest;
	char	*temp;
    int     i;
    
	tmp = ft_split(path, ":");
	if (!tmp)
        exit(1);
	i = 0;
	while (tmp[i])
	{
		temp = ft_strjoin(tmp[i], "/");
		dest = ft_strjoin(temp, cmd);
		if (access(dest, X_OK) == 0)
        return (dest);
		i++;
	}
	exit(1);
}



char	*get_path(t_env_list *env, char *cmd)
{
	int		i;

	if (!env || !cmd)
		return (NULL);
	i = 0;
	if (access(cmd, X_OK) == 0)
		return (cmd);
	else if (access(cmd, X_OK) == -1 && (ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "/", 1) == 0))
		return (NULL);
    while (env)
    {
        if (!ft_strncmp(env->var, "PATH=", 5))
            break;
        env = env->next;
    }
	return (prepare_path(env->var + 5, cmd));
}


char **env_list_to_array(t_env_list *list)
{
    int count;
    int i;
    char **env;

    count = env_size(list);
    env = (char **)gc_malloc(sizeof(char *) * (count + 1), 1);
    if (!env)
        return NULL;
    env[count] = NULL;
    i = -1;
    while (++i < count)
    {
        env[i] = ft_strdup(list->var);
        if (!env[i])
        {
            gc_malloc(0, 0);
            return NULL;
        }
        list = list->next;
    }
    return env;
}



int is_builtin(char *cmd)
{
    return (!ft_strncmp(cmd, "export", 7) || !ft_strncmp(cmd, "env", 4) ||
            !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "cd", 3) ||
            !ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "pwd", 4));
}


void exec_builtin(t_exc_lits *cmd, t_data_parsing *data_exec)
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


// void	do_pipe(t_data_parsing *data_exec, t_exc_lits *cmd_lst, int *input_fd)
// {
// 	int	fd[2];

// 	if (cmd_lst->next && pipe(fd) == -1)
// 	{
// 		perror("pipe");
// 		return ;
// 	}
// 	do_command(cmd_lst, data_exec, fd, *input_fd);
// 	if (cmd_lst->next)
// 	{
// 		close(fd[1]);
// 		*input_fd = fd[0];
// 	}
// }

void handle_redirection(t_data_parsing *data)
{
    if (!data->head_file)
    {
        printf("nofile\n");
        return ;
    }
    printf("here\n");
    while (data->head_file)
    {
        printf("%s [%u]\n", data->head_file->file, data->head_file->type);
        data->head_file = data->head_file->next;
    }
}

void single_cmd(t_data_parsing *data)
{
    char *path;
    char **env;
    int pid;

    handle_redirection(data);
    if (is_builtin(data->head_exe->cmd[0]))
    {
        exec_builtin(data->head_exe, data);
        return ;
    }    
    pid = fork();
    if (pid == 0)
    {
        path = get_path(data->e, data->head_exe->cmd[0]);
        
        env = env_list_to_array(data->e);
        if (env)
            execve(path, data->head_exe->cmd, env);
        else 
            printf("something went wrong !\n");
    }
    waitpid(pid, NULL, 0);
}

void	execution(t_data_parsing *data_exec)
{
	// int			input_fd;
    t_file	*cmd_lst;
    // int i = 0;

	// input_fd = 0;
	cmd_lst = data_exec->head_file;
	if (!cmd_lst)
		return ;

    // handle_redirection(data_exec);
    // if (cmds_size(data_exec->head_exe) == 1)
    //     single_cmd(data_exec);

	while (cmd_lst)
	{
        // i = 0;

        printf("[%s] ", cmd_lst->file);

        printf("\n");
		// do_pipe(data_exec, cmd_lst, &input_fd);
		cmd_lst = cmd_lst->next;
	}
}














// void execution(t_data_parsing *data_exec)
// {

//     if (data_exec->head_exe->cmd)
//     {
//         if (!ft_strncmp(data_exec->head_exe->cmd[0], "export", 7))
//             f_export(data_exec->head_exe->cmd, data_exec->e);
        
//         if (!ft_strncmp(data_exec->head_exe->cmd[0], "env", 4))
//             f_env(data_exec->e);
    
//         if (!ft_strncmp(data_exec->head_exe->cmd[0], "unset", 6))
//         {
//             f_unset(&data_exec->e, data_exec->head_exe->cmd);
//             // print_export(data_exec->e);
//         }
//         if (!ft_strncmp(data_exec->head_exe->cmd[0], "cd", 3))
//             f_cd(data_exec->head_exe->cmd, data_exec->e);
        
//         if ( !ft_strncmp(data_exec->head_exe->cmd[0], "echo", 5))  
//             f_echo(data_exec->head_exe->cmd);
    
//         if (!ft_strncmp(data_exec->head_exe->cmd[0], "pwd", 4))  
//             f_pwd();
//     }

// }