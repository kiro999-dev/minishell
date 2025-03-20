/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utlis2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:56:28 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/20 01:09:50 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void copy_the_splited_string(char **split,char **cmd,int *i,int j)
{
	while (split[j])
	{
		cmd[*i] = split[j];
		(*i)++;
		j++;
	}
	(*i)--;
}
void	join_the_strings(t_toknes_list **current, char **cmd, int *i)
{
	*current = (*current)->next;
	while (*current && (*current)->join_me && !(*current)->split_it)
	{
		if (*i < 0)
			*i = 0;
		cmd[*i] = ft_strjoin(cmd[*i], (*current)->val);
		*current = (*current)->next;
	}
	(*i)++;
}