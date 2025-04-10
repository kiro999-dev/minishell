#include "../minishell.h"


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
    if (cmd->cmd)
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
    // printf("-> %s\n", path);
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
    int         status;
    
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
    {
        waitpid(pid, &status, 0);
        check_exit(status);
    }
    if (head->heredoc_filename)
        unlink(head->heredoc_filename);
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


static void parent_process(int *prev_pipe_in, int pipe_fd[2], t_exc_lits **cmd_lst)
{
    if (*prev_pipe_in != -1)
        close(*prev_pipe_in);
    if ((*cmd_lst)->next)
    {
        close(pipe_fd[1]);
        *prev_pipe_in = pipe_fd[0];
    }
    if ((*cmd_lst)->heredoc_filename)
        unlink((*cmd_lst)->heredoc_filename);
    *cmd_lst = (*cmd_lst)->next;
}

void wait_multiple_childs(t_exc_lits *lst, int *pids, int cmd_len)
{
    int status;
    int i;

    if (!lst || !pids || !cmd_len)
        return ;
    i = -1;
    while (++i < cmd_len)
    {
        waitpid(pids[i], &status, 0);
        if (check_exit(status))
            break;
    }
}

static void execute_pipeline(t_data_parsing *data_exec)
{
    t_exc_lits *cmd_lst ;
    int pipe_fd[2];
    int prev_pipe_in = -1;
    pid_t *pids;
    int cmd_size;
    int i;

    prev_pipe_in = -1;
    cmd_lst = data_exec->head_exe;
    cmd_size = cmds_size(cmd_lst);
    pids = gc_malloc(sizeof(int) * cmd_size, 1);
    if (!pids)
        return ;
    i = -1;
    while (cmd_lst && ++i < cmd_size)
    {
        if (cmd_lst->next && pipe(pipe_fd) == -1)
        {
            perror("minishell: pipe");
            return;
        }
        signal(SIGINT, SIG_IGN);
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("minishell: fork");
            return;
        }
        if (pids[i] == 0)
            child_process(cmd_lst, data_exec, prev_pipe_in, pipe_fd);
        else
            parent_process(&prev_pipe_in, pipe_fd, &cmd_lst);
    }
    if (prev_pipe_in != -1)
        close(prev_pipe_in);
    wait_multiple_childs(data_exec->head_exe, pids, cmd_size);
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


