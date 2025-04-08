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
        exit(1);
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

    if (!cmd)
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
    if (cmd->heredoc_filename != NULL)
        last_input_fd = open(cmd->heredoc_filename, O_RDONLY);
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



void run_command(t_env_list *e, t_exc_lits *cmd_lst, int pid)
{
    char *path;
    char **env;

    default_signals();
    path = get_path(e, cmd_lst->cmd[0]);
    env = env_list_to_array(e);
    if (!path || !env)
    {
        printf("minishell: %s: command not found\n", cmd_lst->cmd[0]);
        if (pid == 0)
            exit(1);
        else 
            return ;
    }
    handle_redirection(cmd_lst);
    execve(path, cmd_lst->cmd, env);
    printf("minishell: %s: command not found\n", cmd_lst->cmd[0]);
    if (pid == 0)
        exit(1);
    return ;
}


void single_cmd(t_data_parsing *data_exec)
{
    t_exc_lits	*head;
    int         pid;
    
    head = data_exec->head_exe;
    if (!head || !head->cmd)
        return;
    if (is_builtin(head->cmd[0]))
    {
        builtins_process(data_exec);
        return ;
    }
    signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == 0)
        run_command(data_exec->e, head, pid);
    else
        check_exit(pid);
}


int process_heredocs(t_exc_lits *cmd,t_env_list *e)
{
    t_list_here_doc *herdoc_head;
    int fd;
    char *line;
    char *filename;
    int i;

    i = 0;
    heredoc_signals();
    while (cmd && exit_herdoc(0, 0) != 1)
    {
        filename = ft_strjoin("/tmp/minishell_heredoc_", ft_itoa(i));
        herdoc_head = cmd->head_here_doc;
        while (herdoc_head && exit_herdoc(0, 0) != 1)
        {
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                // need to be handled !! it hang up
                break;
            }
            while (1)
            {
                line = readline("> ");
                if (exit_herdoc(0, 0))
                {
                    close(fd);
                    free(line);
                    unlink(filename);
                    break;
                }
                if (!line || !ft_strcmp(line, herdoc_head->limtter))
                {
                    free(line);
                    break;
                }
                if(check_expand_h(&line,e))
                {
                    printf("%s\n",line);
                    write(fd, line, ft_strlen(line));
                    write(fd, "\n", 1);
                }
                else
                {
                    write(fd, line, ft_strlen(line));
                    write(fd, "\n", 1);
                    free(line);
                }
            }
            close(fd);
            cmd->heredoc_filename = filename; 
            herdoc_head = herdoc_head->next;
        }
        cmd = cmd->next;
        i++;
    }
    if (exit_herdoc(0, 0) == 1)
        i = 0;
    else 
        i = 1; 
    signals_handling();
    return (i); 
}


static void child_process(t_exc_lits *cmd, t_data_parsing *data_exec, int prev_pipe_in, int pipe_fd[2])
{
    int hd_fd;


    if (prev_pipe_in != -1)
    {
        dup2(prev_pipe_in, STDIN_FILENO);
        close(prev_pipe_in);
    }
    else if (cmd->heredoc_filename)
    {
        hd_fd = open(cmd->heredoc_filename, O_RDONLY);
        if (hd_fd != -1)
        {
            dup2(hd_fd, STDIN_FILENO);
            close(hd_fd);
        }
    }
    if (cmd->next)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    if (is_builtin(cmd->cmd[0]))
    {
        handle_redirection(cmd);
        exec_builtin(cmd, data_exec);
        exit(0);
    }
    else
        run_command(data_exec->e, cmd, 0);
}


static void parent_process(int *prev_pipe_in, int pipe_fd[2], t_exc_lits **cmd_lst, int pid)
{
    check_exit(pid);
    if (*prev_pipe_in != -1)
        close(*prev_pipe_in);
    if ((*cmd_lst)->next)
    {
        close(pipe_fd[1]);
        *prev_pipe_in = pipe_fd[0];
    }
    *cmd_lst = (*cmd_lst)->next;
}

static void execute_pipeline(t_data_parsing *data_exec)
{
    t_exc_lits *cmd_lst ;
    int pipe_fd[2];
    int prev_pipe_in = -1;
    pid_t pid;

    prev_pipe_in = -1;
    cmd_lst = data_exec->head_exe;
    while (cmd_lst)
    {
        if (cmd_lst->next && pipe(pipe_fd) == -1)
        {
            perror("minishell: pipe");
            return;
        }
        signal(SIGINT, SIG_IGN);
        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            return;
        }
        if (pid == 0)
            child_process(cmd_lst, data_exec, prev_pipe_in, pipe_fd);
        else
            parent_process(&prev_pipe_in, pipe_fd, &cmd_lst, pid);
    }
    if (prev_pipe_in != -1)
        close(prev_pipe_in);
    while (wait(NULL) > 0);
}



void execution(t_data_parsing *data_exec)
{
    t_exc_lits *cmd_lst;

    cmd_lst = data_exec->head_exe;
    if (!cmd_lst)
        return;
    if (!process_heredocs(cmd_lst,data_exec->e))
        return ;
    if (cmds_size(cmd_lst) == 1)
        single_cmd(data_exec);
    else
        execute_pipeline(data_exec);
}


