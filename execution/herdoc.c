/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:00:04 by onajem            #+#    #+#             */
/*   Updated: 2025/04/22 19:00:04 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*remove_q_d_h(char *s)
{
	int		i;
	char	*cpy;
	char	*temp;
	int		j;

	i = 0;
	cpy = ft_strdup("");
	j = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			process_quote(s, &i, &cpy, '\'');
		else if (s[i] == '\"')
			process_quote(s, &i, &cpy, '\"');
		else
		{
			temp = process_unquoted(s, &i);
			while (temp[j])
				cpy = join_character(cpy, temp[j++]);
		}
	}
	return (cpy);
}

int	is_qouted(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	herdoc_file(char *filename, t_exc_lits *cmd)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	cmd->heredoc_fd = open(filename, O_RDONLY | O_CREAT | O_TRUNC, 0644);
	unlink(filename);
	return (fd);
}

void	fill_herdoc_file(int fd, char *limtter, t_exc_lits *cmd, t_env_list *e)
{
	char	*line;
	int		flag;

	flag = 0;
	while (1)
	{
		line = readline("> ");
		if (exit_herdoc(0, 0))
		{
			(close(cmd->heredoc_fd), close(fd), free(line));
			break ;
		}
		if (is_qouted(limtter))
			flag = 1;
		limtter = ft_strdup(remove_q_d_h(limtter));
		if (!line || !ft_strcmp(line, limtter))
		{
			(close(fd), free(line));
			break ;
		}
		if (!flag && check_expand_h(&line, e))
			(write(fd, line, ft_strlen(line)), write(fd, "\n", 1));
		else
			(write(fd, line, ft_strlen(line)), write(fd, "\n", 1), free(line));
	}
}

int	process_heredocs(t_exc_lits *cmd, t_env_list *e, int fd)
{
	t_list_here_doc	*herdoc_head;
	char			*filename;

	heredoc_signals();
	while (cmd && exit_herdoc(0, 0) != 1)
	{
		filename = generate_random_filename();
		herdoc_head = cmd->head_here_doc;
		while (herdoc_head && exit_herdoc(0, 0) != 1)
		{
			fd = herdoc_file(filename, cmd);
			if (fd == -1 || cmd->heredoc_fd == -1)
			{
				exit_status(1, 1);
				break ;
			}
			fill_herdoc_file(fd, herdoc_head->limtter, cmd, e);
			close(fd);
			if (herdoc_head->next)
				close(cmd->heredoc_fd);
			herdoc_head = herdoc_head->next;
		}
		cmd = cmd->next;
	}
	return (signals_handling(), !(exit_herdoc(0, 0) == 1));
}
