/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:26:37 by onajem            #+#    #+#             */
/*   Updated: 2025/04/22 16:26:37 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtins_process(t_data_parsing *data)
{
	t_exc_lits	*cmd;
	int			saved_stdin;
	int			saved_stdout;

	cmd = data->head_exe;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("minishell: dup");
		return ;
	}
	if (handle_redirection(cmd))
	{
		exit_status(1, 1);
		return ;
	}
	exec_builtin(cmd, data, 0);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("minishell: stdin restore");
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("minishell: stdout restore");
	close(saved_stdin);
	close(saved_stdout);
}

void	run_command(t_env_list *e, t_exc_lits *cmd_lst, int pid)
{
	char	*path;
	char	**env;

	default_signals();
	path = get_path(e, cmd_lst->cmd[0]);
	env = env_list_to_array(e);
	if (!path || !env)
	{
		if (cmd_lst->cmd[0])
			handle_file_error(cmd_lst->cmd[0], 0);
		if (pid == 0)
			(close_fds(), gc_malloc(0, 0), exit(1));
		else
			return ;
	}
	execve(path, cmd_lst->cmd, env);
	if (handle_exe_files(path) == 0)
		(close_fds(), gc_malloc(0, 0), exit(0));
	else if (handle_exe_files(path) == 2)
		(handle_file_error(path, 2), close_fds(), gc_malloc(0, 0), exit(126));
	else
		print_error(cmd_lst->cmd[0], ": command not found\n", NULL);
	if (pid == 0)
		(close_fds(), gc_malloc(0, 0), exit(127));
	return ;
}

int	check_no_cmd(t_exc_lits *head, t_env_list *e)
{
	(void)e;
	if (!head->cmd && !head->head_files)
	{
		close_fds();
		exit_status(0, 1);
		return (0);
	}
	else if (!head->cmd && head->head_files)
	{
		exit_status(handle_redirection(head), 1);
		close_fds();
		return (exit_status(0, 0));
	}
	return (-1);
}

void	single_cmd(t_data_parsing *data_exec)
{
	t_exc_lits	*head;
	int			pid;
	int			status;

	status = 0;
	head = data_exec->head_exe;
	if (!head || check_no_cmd(head, data_exec->e) != -1)
		return ;
	if (is_builtin(head->cmd[0]))
		return (builtins_process(data_exec), (void)0);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		if (handle_redirection(head))
			(close_fds(), gc_malloc(0, 0), exit(1));
		run_command(data_exec->e, head, pid);
	}
	waitpid(pid, &status, 0);
	if (check_exit(status))
		printf("\n");
	if (head->heredoc_fd != -1)
		close(head->heredoc_fd);
}

void	execution(t_data_parsing *data_exec)
{
	t_exc_lits	*cmd_lst;
	int			*pids;
	int			cmd_size;
	int			fd_herdoc;

	cmd_lst = data_exec->head_exe;
	if (!cmd_lst)
		return ;
	fd_herdoc = 0;
	if (!process_heredocs(cmd_lst, data_exec->e, fd_herdoc))
		return ;
	if (cmds_size(cmd_lst) == 1)
		single_cmd(data_exec);
	else
	{
		cmd_size = cmds_size(cmd_lst);
		pids = gc_malloc(sizeof(int) * cmd_size, 1);
		if (!pids)
			return ;
		execute_pipeline(data_exec, pids, cmd_size);
	}
}
