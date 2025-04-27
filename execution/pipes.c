/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:58:23 by onajem            #+#    #+#             */
/*   Updated: 2025/04/24 18:08:01 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redirection(t_exc_lits *cmd)
{
	t_file	*file;
	int		last_input_fd;
	int		last_output_fd;
	int		fail;

	last_input_fd = -1;
	last_output_fd = -1;
	fail = 0;
	if (!cmd)
		return (1);
	file = cmd->head_files;
	while (file && fail == 0)
	{
		if (file->type == IS_FILE_IN)
			fail = apply_input_redirection(&last_input_fd, file);

		if (file->type == IS_FILE_OUT || file->type == IS_FILE_APPEND)
			fail = apply_output_redirection(&last_output_fd, file, 1);
		file = file->next;
	}
	if (fail)
		return (1);
	if (cmd->heredoc_fd != -1 && cmd->priority == 2)
		last_input_fd = cmd->heredoc_fd;
	

	if (cmd->cmd)
		set_final_redirections(last_input_fd, last_output_fd);
	else if (last_input_fd != -1)
		close(last_input_fd);
	else if (last_output_fd != -1)
		close(last_output_fd);
	return (0);
}

void	child_process(t_exc_lits *cmd, t_data_parsing *data, int in, int fd[2])
{
	int	exit_child;

	if (handle_redirection(cmd))
		(close_fds(), exit(1));
	if (cmd->heredoc_fd == -1 && !check_in_out(cmd->head_files, 0) && in != -1)
		(dup2(in, STDIN_FILENO), close(in));
	if (cmd->next && !check_in_out(cmd->head_files, 1))
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	exit_child = check_no_cmd(cmd, data->e);
	if (exit_child != -1)
		(close_fds(), exit(exit_child));
	if (is_builtin(cmd->cmd[0]))
	{
		if (handle_redirection(cmd))
			(close_fds(), exit(1));
		exec_builtin(cmd, data, 1);
		(close_fds(), exit(0));
	}
	else
		run_command(data->e, cmd, 0);
}

void	parent_process(int *prev_pipe_in, int pipe_fd[2], t_exc_lits **cmd_lst)
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

void	wait_multiple_childs(t_exc_lits *lst, int *pids, int cmd_len)
{
	int	status;
	int	i;
	int	new_line;

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

void	execute_pipeline(t_data_parsing *data_exec, int *pids, int cmd_size)
{
	t_exc_lits	*cmd_lst;
	int			pipe_fd[2];
	int			prev_pipe_in;
	int			i;

	prev_pipe_in = -1;
	cmd_lst = data_exec->head_exe;
	i = -1;
	while (cmd_lst && ++i < cmd_size)
	{
		if (cmd_lst->next && pipe(pipe_fd) == -1)
			return ;
		signal(SIGINT, SIG_IGN);
		pids[i] = fork();
		if (pids[i] == -1)
			return ;
		if (pids[i] == 0)
			child_process(cmd_lst, data_exec, prev_pipe_in, pipe_fd);
		else
			parent_process(&prev_pipe_in, pipe_fd, &cmd_lst);
	}
	if (prev_pipe_in != -1)
		close(prev_pipe_in);
	wait_multiple_childs(data_exec->head_exe, pids, cmd_size);
}
