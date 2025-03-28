#include "../minishell.h"

char *prepare_path(char *path, char *cmd)
{
    char **tmp;
    char *dest;
    char *temp;
    int i;

    if (!path || !cmd)
        return NULL;
    tmp = ft_split(path, ":");
    if (!tmp)
        return NULL;
    i = -1;
    while (tmp[++i])
    {
        temp = ft_strjoin(tmp[i], "/");
        dest = ft_strjoin(temp, cmd);  
        if (access(dest, F_OK) == 0)
        {
            if (access(dest, X_OK) == 0)
                return dest;
            return NULL;
        }
    }
    return NULL;
}

int cmd_in_out_redirection(t_exc_lits *cmd, int red)
{
    t_file *file = cmd->head_files;
    while (file)
    {
        if (red == 0 && file->type == IS_FILE_IN)
            return (1);
        if (red && (file->type == IS_FILE_APPEND || file->type == IS_FILE_OUT))
            return (1);
        file = file->next;
    }
    return (0);
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
    if (!env)
    {
        printf("minishell : PATH not set up!\n");
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
			!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "pwd", 4) ||
            !ft_strncmp(cmd, "exit", 5));
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
		f_cd(cmd->cmd, &data_exec->e);
	else if (!ft_strncmp(cmd->cmd[0], "echo", 5))
		f_echo(cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "pwd", 4))
		f_pwd(data_exec->e);
    else if (!ft_strncmp(cmd->cmd[0], "exit", 5))
        f_exit(cmd->cmd, data_exec);
}

void apply_input_redirection(int *last_input_fd, const char *file)
{
    if (*last_input_fd != -1)
        close(*last_input_fd);

    *last_input_fd = open(file, O_RDONLY);
    if (*last_input_fd == -1)
        exit(1) ;
}

void apply_output_redirection(int *last_output_fd, const char *file, t_TOKENS type)
{
    if (*last_output_fd != -1)
        close(*last_output_fd);

    if (type == IS_FILE_OUT )  
        *last_output_fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    else if(type == IS_FILE_APPEND)
        *last_output_fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);

    if (*last_output_fd == -1)
        exit(1) ;
}


void set_final_redirections(int last_input_fd, int last_output_fd)
{
    if (last_input_fd != -1)
    {
        dup2(last_input_fd, STDIN_FILENO);
        close(last_input_fd);
    }
    
    if (last_output_fd != -1)
    {
        dup2(last_output_fd, STDOUT_FILENO);
        close(last_output_fd);
    }
}


void handle_redirection(t_exc_lits *cmd)
{
    t_file *file;
    int last_input_fd = -1;
    int last_output_fd = -1;

    if (!cmd || !cmd->head_files)
        return;
    file = cmd->head_files;
    while (file)
    {
        if (file->type == IS_FILE_IN)
            apply_input_redirection(&last_input_fd, file->file);
        else if (file->type == IS_FILE_OUT || file->type == IS_FILE_APPEND)
            apply_output_redirection(&last_output_fd, file->file, file->type);

        file = file->next;
    }
    set_final_redirections(last_input_fd, last_output_fd);
}

void builtins_process(t_data_parsing *data)
{
    t_exc_lits *cmd = data->head_exe;
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    
    if (saved_stdin == -1 || saved_stdout == -1) {
        perror("minishell: dup");
        return;
    }

    handle_redirection(cmd);
    
    exec_builtin(cmd, data);
    
    if (dup2(saved_stdin, STDIN_FILENO) == -1)
        perror("minishell: stdin restore");
    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
        perror("minishell: stdout restore");
    
    close(saved_stdin);
    close(saved_stdout);
}


void single_cmd(t_data_parsing *data_exec)
{
    t_exc_lits	*head;
    int         pid;
    
    head = data_exec->head_exe;
    if (!head || !head->cmd)
        return;
    printf("gg\n"); 
    if (is_builtin(head->cmd[0]))
    {
        builtins_process(data_exec);
        return ;
    }
    pid = fork();
    if (pid == 0)
    {
        handle_redirection(head);
        char *path = get_path(data_exec->e, head->cmd[0]);
        char **env = env_list_to_array(data_exec->e);
        if (!path || !env)
        {
            printf("Command not found or environment error!\n");
            return ;
        }
        execve(path, head->cmd, env);
        printf("execve error!\n");
        return ;
    }
    waitpid(pid, NULL, 0);
}



void execution(t_data_parsing *data_exec)
{
    t_exc_lits *cmd_lst = data_exec->head_exe;
    int pipe_fd[2];
    int prev_pipe_in = -1;
    pid_t pid;
    
    if (!cmd_lst)
        return;

    if (cmds_size(cmd_lst) == 1)
    {
        single_cmd(data_exec);
        return;
    }

    while (cmd_lst)
    {
        if (cmd_lst->next)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("minishell: pipe");
                return;
            }
        }

        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            return;
        }

        if (pid == 0) 
        {
            // Set up input from previous command
            if (prev_pipe_in != -1)
            {
                dup2(prev_pipe_in, STDIN_FILENO);
                close(prev_pipe_in);
            }

            // Set up output to next command (if not last command)
            if (cmd_lst->next)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            handle_redirection(cmd_lst);

            if (is_builtin(cmd_lst->cmd[0]))
            {
                exec_builtin(cmd_lst, data_exec);
                exit(0);
            }
            else
            {
                char *path = get_path(data_exec->e, cmd_lst->cmd[0]);
                char **env = env_list_to_array(data_exec->e);
                if (!path || !env)
                {
                    printf("minishell: %s: command not found\n", cmd_lst->cmd[0]);
                    exit(127);
                }
                execve(path, cmd_lst->cmd, env);
                exit(126);
            }
        }
        else 
        {
            if (prev_pipe_in != -1)
                close(prev_pipe_in);

            // If not last command, save read end for next command
            if (cmd_lst->next)
            {
                close(pipe_fd[1]); // Close write end
                prev_pipe_in = pipe_fd[0];
            }

            cmd_lst = cmd_lst->next;
        }
    }

    if (prev_pipe_in != -1)
        close(prev_pipe_in);

    while (wait(NULL) > 0);
}
