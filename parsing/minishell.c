/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:16:37 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/21 20:46:04 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	data_init(t_data_parsing *data, char **env, int flag)
{
	data->buff = NULL;
	data->head_exe = NULL;
	data->gc_head = NULL;
	data->head_file = NULL;
	if (flag)
		data->e = init_env(env);
	data->head_toknez = NULL;
}

int	parsing(t_data_parsing *data)
{
	lex(data->buff, &data->head_toknez);
	if (!check_syntax(data->head_toknez))
	{
		expanding(data->head_toknez, data->e);
		generate_list(data->head_toknez, &data->head_exe);
		free(data->buff);
		return (1);
	}
	else
	{
		exit_status(2, 1);
		free(data->buff);
		return (0);
	}
}

void	start_minishell(t_data_parsing *data, char **env)
{
	if (parsing(data))
	{
		execution(data);
		data_init(data, env, 0);
	}
	else
		data_init(data, env, 0);
}

int	main(int argc, char **argv, char **env)
{
	t_data_parsing	data;
	char			*tmp;

	data_init(&data, env, 1);
	(void)argc;
	(void)argv;
	tmp = getcwd(NULL, 0);
	if (!tmp)
		write(2, "error retrieving current directory: getcwd failed\n", 51);
	signals_handling();
	data.p_pwd = ft_strdup(tmp);
	free(tmp);
	while (1)
	{
		data.buff = readline("$minishell> ");
		if (data.buff == NULL)
			(printf("exit\n"), close_fds(), gc_malloc(0, 0)),
					exit(exit_status(0, 0));
		if (data.buff[0] != '\0')
			add_history(data.buff);
		start_minishell(&data, env);
	}
	gc_malloc(0, 0);
	return (0);
}
