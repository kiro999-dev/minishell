/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:58:48 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/21 22:55:14 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	expand_in_double_quotes(t_toknes_list *head, t_env_list *e, int *i)
{
	int	dq;

	dq = 1;
	(*i)++;
	if (head->val[*i] == '\"')
		dq = 0;
	while (dq && *i < ft_strlen(head->val) && head->val[*i])
	{
		if (head->val[*i] == '$')
		{
			while (head->val[*i] && head->val[*i] == '$')
				(*i)++;
			if (head->val[*i] == '?')
				handle_dollar_expansion(i, e, head, 0);
			else if (!not_character_expand(head->val[*i]))
				handle_dollar_expansion(i, e, head, 0);
		}
		else if (head->val[*i] == '\"')
			dq = 0;
		else
			(*i)++;
	}
}

static void	expand_plain(t_toknes_list *head, t_env_list *e, int *i,
	int flag_split)
{
	if (flag_split)
		head->split_it = 1;
	if (split_t2condi(head->val, 0) && head->val[0] != '$')
		head->split_it2 = 1;
	else
		head->split_it = 1;
	while (head->val[*i] && head->val[*i] == '$')
		(*i)++;
	if (head->val[*i] == '?')
		handle_dollar_expansion(i, e, head, 1);
	else if (!not_character_expand(head->val[*i]))
		handle_dollar_expansion(i, e, head, 1);
}

int	check_is_expandig(t_toknes_list *head, t_env_list *e, int flag_split)
{
	int	i;
	int	q;

	i = 0;
	q = 0;
	while (i < ft_strlen(head->val) && head->val[i])
	{
		if (head->val[i] == '\"')
			expand_in_double_quotes(head, e, &i);
		else if (head->val[i] == '\'')
		{
			q = 1;
			skip_q_expand(head->val, &i, &q);
		}
		else if (head->val[i] == '$'
			&& !(head->val[i + 1] >= '0' && head->val[i + 1] <= '9'))
		{
			head->ambiguous = 1;
			expand_plain(head, e, &i, flag_split);
		}
		i++;
	}
	return (i);
}

void	check_expand(t_toknes_list *head, t_env_list *e, int flag_split)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(head->val);
	while (head && head->val && i < len)
	{
		if (head->val[i] == '$' && head->type != LIMTER)
			i = check_is_expandig(head, e, flag_split);
		len = ft_strlen(head->val);
		i++;
	}
}

void	expanding(t_toknes_list *token_head, t_env_list *e)
{
	t_toknes_list	*head;
	int				flag;

	flag = 1;
	head = token_head;
	if (head)
	{
		if (!ft_strcmp(head->val, "export"))
			flag = 0;
	}
	while (head)
	{
		check_expand(head, e, flag);
		remove_q_d(head);
		head = head->next;
	}
}
