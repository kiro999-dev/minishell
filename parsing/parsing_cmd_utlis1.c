/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utlis1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:42:42 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/20 01:08:35 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_strcmp(char *s1 ,char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if(s1[i] != s2[i])
			return(1);
		i++;
	}
	if(s1[i] != s2[i])
		return(1);
	return (0);
	
}
int count_cmd_tokens(t_toknes_list *token)
{
	int count = 0;
	int flag = 0;
	while ((token && token->type != PIPE ))
	{
		if (token->type == WORD || token->type == CMD)
		{
			if (token->split_it)
			{
				if (token->val)
					count += counting_words(token->val, " \n\t");
				if (token->join_me)
					count--;
				flag = 1;
			}
			else if (!token->join_me && !flag)
				count++;
			else if (token->join_me && !flag)
				count++;
		}
		token = token->next;
		flag = 0;
	}
	return count;
}

int counting(char **spilt)
{
	int i = 0;
	if (!spilt)
		return (0);
	while (spilt[i])
		i++;
	return (i);
}

char **realloc_cmd_array(char **cmd, int current_count, int extra)
{
	int new_size, i;
	char **new_cmd;
	i = 0;
	new_size = current_count + extra + 1;
	new_cmd = gc_malloc(sizeof(char *) * new_size,1);
	if (!new_cmd)
		return (NULL);
	while (i < current_count)
	{
		new_cmd[i] = cmd[i];
		i++;
	}
	new_cmd[current_count] = NULL;
	return (new_cmd);
}
char **cmd_int(int count)
{
	char **cmd;
	int k = 0;
	cmd = gc_malloc(sizeof(char *) * (count + 1),1);
	while (k < count)
	{
		cmd[k] = NULL;
		k++;
	}
	return (cmd);
}
