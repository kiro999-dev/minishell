#include "../minishell.h"


int handle_redirection(t_exc_lits *cmd)
{
    t_file *file;
    int last_input_fd = -1;
    int last_output_fd = -1;

    if (!cmd)
        return (1);
    file = cmd->head_files;

    apply_input_redirection(&last_input_fd, file);
    if (cmd->heredoc_fd != -1)
        last_input_fd = cmd->heredoc_fd;
    if (cmd_in_out_redirection(file, 0) && last_input_fd == -1)
        return (1);
    if (apply_output_redirection(&last_output_fd, file))
        return (1);
    
    if (cmd->cmd)
        set_final_redirections(last_input_fd, last_output_fd);
    else
    {
        close(last_input_fd);
        close(last_output_fd);
    }
    return (0);
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

    if (handle_redirection(cmd))
    {
        exit_status(1, 1);
        return;
    }

    exec_builtin(cmd, data, 0);

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
        handle_cd_error(cmd_lst->cmd[0], 0);
        if (pid == 0)
            exit(1);
        else
            return ;
    }
    execve(path, cmd_lst->cmd, env);
    handle_cd_error(cmd_lst->cmd[0], 0);
    if (pid == 0)
        exit(127);
    return ;
}


int check_no_cmd(t_exc_lits *head, t_env_list *e)
{
    int out;

    if (!head->cmd && !head->head_files)
    {
        exit_status(0, 1);
        return (0);
    }
    else if (!head->cmd && head->head_files)
    {
        exit_status(handle_redirection(head), 1);
        return (exit_status(0, 0));
    }
    if (head->cmd && !is_builtin(head->cmd[0]) && !get_path(e, head->cmd[0]))
    {
        apply_output_redirection(&out, head->head_files);
        exit_status(127, 1);
        close(out);
        return (127);
    }
    return (-1);
}


void single_cmd(t_data_parsing *data_exec)
{
    t_exc_lits	*head;
    int         pid;
    int         status;
    
    head = data_exec->head_exe;
    if (!head || check_no_cmd(head, data_exec->e) != -1)
        return;
    if (is_builtin(head->cmd[0]))
    {
        builtins_process(data_exec);
        return ;
    }
    signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == 0)
    {
        if(handle_redirection(head))
            exit(1);
        run_command(data_exec->e, head, pid);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (check_exit(status))
            printf("\n");
    }
}

static void child_process(t_exc_lits *cmd, t_data_parsing *data_exec, int prev_pipe_in, int pipe_fd[2])
{
    int exit_child;

    if (handle_redirection(cmd))
        exit(1);

    if (cmd->heredoc_fd == -1 && !cmd_in_out_redirection(cmd->head_files, 0) && prev_pipe_in != -1)
    {
        dup2(prev_pipe_in, STDIN_FILENO);
        close(prev_pipe_in);
    }
    if (cmd->next && !cmd_in_out_redirection(cmd->head_files, 1))
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    exit_child = check_no_cmd(cmd, data_exec->e);
    if (exit_child != -1)
        exit(exit_child);
    
    if (is_builtin(cmd->cmd[0]))
    {
        if (handle_redirection(cmd))
            exit(1);
        exec_builtin(cmd, data_exec, 1);
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
    
    *cmd_lst = (*cmd_lst)->next;
}

void wait_multiple_childs(t_exc_lits *lst, int *pids, int cmd_len)
{
    int status;
    int i;
    int new_line;

    if (!lst || !pids || !cmd_len)
        return ;
    i = -1;
    status = 0;
    while (++i < cmd_len)
    {
        waitpid(pids[i], &status, 0);
        new_line = check_exit(status);
    }
    if (new_line)
        printf("\n");
    while (lst)
    {
        if (lst->heredoc_fd != -1)
            close(lst->heredoc_fd);
        lst = lst->next;
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
    if (!process_heredocs(cmd_lst, data_exec->e))
        return ;
    if (cmds_size(cmd_lst) == 1)
        single_cmd(data_exec);
    else
        execute_pipeline(data_exec);
}


