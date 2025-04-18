/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:58:48 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/18 14:20:24 by zkhourba         ###   ########.fr       */
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
			if (not_character_expand(head->val[*i]))
				break ;
			handle_dollar_expansion(i, e, head, 0);
		}
		else if (head->val[*i] == '\"')
			dq = 0;
		else
			(*i)++;
	}
}

static void	expand_plain(t_toknes_list *head, t_env_list *e, int *i)
{
	if (head->val[0] == '$' || head->val[0] == '=')
		head->split_it = 1;
	else if (split_t2condi(head->val, 0))
		head->split_it2 = 1;
	else
		head->split_it = 1;
	while (head->val[*i] && head->val[*i] == '$')
		(*i)++;
	if (!not_character_expand(head->val[*i]))
		handle_dollar_expansion(i, e, head, 1);
}

int	check_is_expandig(t_toknes_list *head, t_env_list *e)
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
		else if (head->val[i] == '$')
		{
			expand_plain(head, e, &i);
			head->ambiguous = 1;
		}
		i++;
	}
	return (i);
}

int	check_expand(t_toknes_list *head, t_env_list *e)
{
	int	i;
	int	flag;
	int	len;

	i = 0;
	flag = 0;
	len = ft_strlen(head->val);
	while (head && head->val && i < len)
	{
		if (head->val[i] == '$' && head->type != LIMTER)
		{
			i = check_is_expandig(head, e);
			flag = 1;
		}
		len = ft_strlen(head->val);
		i++;
	}
	return (flag);
}

void	expanding(t_toknes_list *token_head, t_env_list *e)
{
	t_toknes_list	*head;

	head = token_head;
	while (head)
	{
		check_expand(head, e);
		remove_q_d(head);
		head = head->next;
	}
}
