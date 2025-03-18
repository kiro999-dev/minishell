/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Updated: 2025/03/04 05:08:57 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>




int parsing(t_data_parsing *data)
{
	lex(data->buff, &data->head_toknez);
		if(!check_syntax(data->head_toknez))
		{
			expanding(data->head_toknez, data->e);
			generate_list(data->head_toknez, &data->head_exe);
			free_gc(&data->gc_head);
			free(data->buff);
			return (1);
		}
	return (0);
}

void data_init(t_data_parsing *data, char **env,int flag)
{
	data->buff=NULL;
	data->head_exe = NULL;
	data->gc_head = NULL;
	data->head_file = NULL;
	if(flag)
		data->e = init_env(env);
	data->head_toknez = NULL;
}

int main(int argc, char **argv, char **env)
{
	
	t_data_parsing data;

	data_init(&data, env,1);
	argc = argc - 1;
	argv[0] = NULL;
	while (1)
	{
		data.buff = readline("> ");
		if (data.buff == NULL)
		{
			free_gc(&data.gc_head);
			break;
		}
		add_history(data.buff);
		if(parsing(&data))
		{
			execution(&data);
			free_gc(&data.gc_head);
			data_init(&data,env,0);
		}
	}
	return 0;
}
