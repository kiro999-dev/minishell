/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:40:57 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 21:28:23 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void process_split_it(t_toknes_list **current, char **cmd, int *i)
{
	int j = 0;
	char **split = ft_split((*current)->val, " \t\n");
	if ((*current)->join_me)
	{
		if (*i > 0)
		{
			(*i)--;
			cmd[*i] = ft_strjoin(cmd[*i], split[0]);
			(*i)++;
		}
		else
		{
			cmd[*i] = ft_strdup(split[0]);
			(*i)++;
		}
		j++;
	}
	while (split[j])
	{
		cmd[*i] = split[j];
		(*i)++;
		j++;
	}
	(*i)--;
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

char **process_split_it2(t_toknes_list **current, char **cmd, int *i, int orig_count)
{
	int j = 0, re_count = 0;
	char **split = ft_split((*current)->val, " \t\n");
	re_count = counting(split);
	if ((*current)->join_me)
	{
		if (*i > 0)
		{
			(*i)--;
			cmd[*i] = ft_strjoin(cmd[*i], split[0]);
			(*i)++;
		}
		else
		{
			cmd[*i] = ft_strdup(split[0]);
			(*i)++;
		}
		j++;
	}
	cmd = realloc_cmd_array(cmd, orig_count, re_count);
	while (split[j])
	{
		cmd[*i] = split[j];
		(*i)++;
		j++;
	}
	(*i)--;
	*current = (*current)->next;
	while (*current && (*current)->join_me && !(*current)->split_it)
	{
		cmd[*i] = ft_strjoin(cmd[*i], (*current)->val);
		*current = (*current)->next;
	}
	(*i)++;
	return (cmd);
}

void process_default(t_toknes_list **current, char **cmd, int *i)
{
	if ((*current)->val)
		cmd[*i] = ft_strdup((*current)->val);
	else
		cmd[*i] = ft_strdup("");
	*current = (*current)->next;
	while (*current && (*current)->join_me && !(*current)->split_it)
	{
		cmd[*i] = ft_strjoin(cmd[*i], (*current)->val);
		*current = (*current)->next;
	}
	(*i)++;
}

char **copy_cmd_tokens(t_toknes_list *token, int count)
{
	t_toknes_list *current;
	char **cmd;
	int i = 0;
	cmd = cmd_int(count);
	current = token;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == CMD)
		{
			if (current->split_it)
				process_split_it(&current, cmd, &i);
			else if (current->split_it2 && cmd[0] && ft_strcmp("export", cmd[0]))
				cmd = process_split_it2(&current, cmd, &i, count);
			else
				process_default(&current, cmd, &i);
		}
		else
			current = current->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

char **pars_cmd(t_toknes_list *token)
{
	int count = count_cmd_tokens(token);
	return (copy_cmd_tokens(token, count));
}