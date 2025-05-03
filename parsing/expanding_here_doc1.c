/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_here_doc1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:42:00 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/21 22:42:25 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_here_doc_nb(t_toknes_list *head1)
{
	int	count;

	count = 0;
	while (head1)
	{
		if (head1->type == HER_DOC)
			count++;
		head1 = head1->next;
	}
	if (count >= 17)
		return (1);
	else
		return (0);
}

static int	expand_in_double_quotes_h(char **val, t_env_list *e, int *i,
		int check)
{
	int	dq;

	dq = 1;
	(*i)++;
	if ((*val)[*i] == '\"')
		dq = 0;
	while (dq && *i < ft_strlen(*val) && (*val)[*i])
	{
		if ((*val)[*i] == '$')
		{
			while ((*val)[*i] && (*val)[*i] == '$')
				(*i)++;
			if ((*val)[*i] == '?')
				check = handle_dollar_expansion_h(i, e, val, check);
			else if (not_character_expand((*val)[*i]))
				break ;
			check = handle_dollar_expansion_h(i, e, val, check);
		}
		else if ((*val)[*i] == '\"')
			dq = 0;
		else
			(*i)++;
	}
	return (1);
}

static int	expand_plain_h(char **val, t_env_list *e, int *i, int check)
{
	while ((*val)[*i] && (*val)[*i] == '$')
		(*i)++;
	if ((*val)[*i] == '?')
		handle_dollar_expansion_h(i, e, val, check);
	else if (!not_character_expand((*val)[*i]))
		handle_dollar_expansion_h(i, e, val, check);
	return (1);
}

int	check_is_expandig_h(char **val, t_env_list *e, int check)
{
	int	i;

	i = 0;
	while (i < ft_strlen(*val) && (*val)[i])
	{
		if ((*val)[i] == '\"')
		{
			check = expand_in_double_quotes_h(val, e, &i, check);
		}
		else if ((*val)[i] == '\'')
		{
			check = expand_in_double_quotes_h(val, e, &i, check);
		}
		else if ((*val)[i] == '$')
		{
			check = expand_plain_h(val, e, &i, check);
		}
		i++;
	}
	return (i);
}

int	check_expand_h(char **val, t_env_list *e)
{
	int	i;
	int	flag;
	int	len;

	i = 0;
	flag = 0;
	len = ft_strlen(*val);
	while (*val && i < len)
	{
		if ((*val)[i] == '$')
		{
			i = check_is_expandig_h(val, e, 0);
			flag = 1;
		}
		len = ft_strlen(*val);
		i++;
	}
	return (flag);
}
