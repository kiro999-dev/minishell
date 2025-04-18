/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:16:37 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/19 17:54:32 by onajem           ###   ########.fr       */
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
		exit_status(2,1);
		free(data->buff);
		return (0);
	}
}

// void	print_list(t_exc_lits *h)
// {
// 	while (h)
// 	{
// 		if (h->head_files)
// 		{
// 			printf("files found -> ");
// 			while (h->head_files)
// 			{
// 				printf("[%s] ", h->head_files->file);
// 				h->head_files = h->head_files->next;
// 			}
// 		}
// 		if(h->head_here_doc)
// 		{
// 			printf("here_doc found -> ");
// 			while (h->head_here_doc)
// 			{
// 				printf(" limter :[%s] ", h->head_here_doc->limtter);
// 				h->head_here_doc = h->head_here_doc->next;
// 			}
// 		}
// 		printf("\n");
// 		h = h->next;
// 	}
// }
// void list_szie(t_exc_lits *head)
// {
// 	int i = 0;
// 	while (head)
// 	{
// 		head = head->next;
// 		i++;
// 	}
// 	printf("size :%d\n",i);
// }

int	main(int argc, char **argv, char **env)
{
	t_data_parsing	data;

	data_init(&data, env, 1);
	(void)argc;
	(void)argv;
	signals_handling();
	char *tmp = getcwd(NULL, 0);
	data.p_pwd = ft_strdup(tmp);
	free(tmp);
	while (1)
	{
		data.buff = readline("$minishell> ");
		if (data.buff == NULL)
		{
			printf("exit\n");
			exit(exit_status(0, 0));
		}
		add_history(data.buff);
		if (parsing(&data))
		{
			execution(&data);
			data_init(&data, env, 0);
		}
		else
			data_init(&data, env, 0);
	}
	gc_malloc(0, 0);
	return (0);
}
