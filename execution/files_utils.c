/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:07:48 by onajem            #+#    #+#             */
/*   Updated: 2025/04/21 18:39:19 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_in_out(t_file *file, int red)
{
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

int	apply_input_redirection(int *last_input_fd, t_file *file)
{
	if (*last_input_fd != -1)
		close(*last_input_fd);
	if (file->file && file->ambigous != 1)
		*last_input_fd = open(file->file, O_RDONLY);
	if (*last_input_fd == -1)
	{
		if (file->ambigous)
			write(2, "minishell: ambiguous redirect\n", 31);
		else
			handle_file_error(file->file, 0);
		return (1);
	}
	return (0);
}

int	apply_output_redirection(int *last_out, t_file *file, int single)
{

	if (*last_out != -1 && single == 1)
		close(*last_out);
	if (file->type == IS_FILE_OUT && file->ambigous != 1)
		*last_out = open(file->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (file->type == IS_FILE_APPEND && file->ambigous != 1)
		*last_out = open(file->file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (*last_out == -1)
	{
		if (file->ambigous)
			write(2, "minishell: ambiguous redirect\n", 31);
		else
			handle_file_error(file->file, 0);
		return (1);
	}
	return (0);
}

void	set_final_redirections(int last_input_fd, int last_output_fd)
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

char	*generate_random_filename(void)
{
	int				random_fd;
	unsigned int	rand_val;
	char			*filename;
	char			random_str[9];

	random_fd = open("/dev/random", O_RDONLY);
	if (random_fd == -1)
		return (NULL);
	if (read(random_fd, &rand_val, sizeof(rand_val)) != sizeof(rand_val))
	{
		close(random_fd);
		return (NULL);
	}
	close(random_fd);
	hexa_format(rand_val, random_str);
	filename = ft_strjoin("/tmp/minishell_heredoc_", random_str);
	return (filename);
}
