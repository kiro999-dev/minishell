/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:16:37 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/22 14:18:54 by onajem           ###   ########.fr       */
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
	return (0);
}

void	print_list(t_exc_lits *h)
{
	int	i;

	i = 0;
	while (h)
	{
		i = 0;
		while (h && h->cmd && h->cmd[i])
		{
			printf("cmd[%d]-->%s\n", i, h->cmd[i]);
			i++;
		}
		h = h->next;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data_parsing	data;

	data_init(&data, env, 1);
	(void)argc;
	(void)argv;
	while (1)
	{
		data.buff = readline("$minishell> ");
		if (data.buff == NULL)
			break ;
		add_history(data.buff);
		if (parsing(&data))
		{
			// check parsin asat ewiwi
			// execution(&data);
			data_init(&data, env, 0);
		}
		else
			data_init(&data, env, 0);
	}
	gc_malloc(0, 0);
	return (0);
}
