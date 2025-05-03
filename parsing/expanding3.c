/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:38:26 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/19 01:15:36 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*build_expand_string(int *i, t_toknes_list *head, int *flag)
{	
	char	*expand;

	expand = ft_strdup("");
	*flag = 0;
	while (head->val[*i])
	{
		if (head->val[*i] == '?')
		{
			(*i)++;
			return (*flag = 1, ft_strdup("?"));
		}
		else if (not_character_expand(head->val[*i])
			&& (head->val[*i] != '\"' && head->val[*i] != '\''))
		{
			*flag = 1;
			break ;
		}
		else if (not_character_expand(head->val[*i]))
			break ;
		expand = join_character(expand, head->val[*i]);
		(*i)++;
	}
	return (expand);
}

void	process_quote(char *s, int *i, char **cpy, char quote)
{
	(*i)++;
	while (s[*i] && s[*i] != quote)
	{
		*cpy = join_character(*cpy, s[*i]);
		(*i)++;
	}
	if (s[*i] == quote)
		(*i)++;
}

char	*process_unquoted(char *s, int *i)
{
	char	*cpy;

	cpy = ft_strdup("");
	while (s[*i] && s[*i] != '\'' && s[*i] != '\"')
	{
		cpy = join_character(cpy, s[*i]);
		(*i)++;
	}
	return (cpy);
}

void	remove_q_d(t_toknes_list *head)
{
	int		i;
	char	*cpy;
	char	*temp;
	int		j;

	if (head->type == LIMTER)
		return ;
	i = 0;
	cpy = ft_strdup("");
	j = 0;
	while (head->val[i])
	{
		j = 0;
		if (head->val[i] == '\'')
			process_quote(head->val, &i, &cpy, '\'');
		else if (head->val[i] == '\"')
			process_quote(head->val, &i, &cpy, '\"');
		else
		{
			temp = process_unquoted(head->val, &i);
			while (temp[j])
				cpy = join_character(cpy, temp[j++]);
		}
	}
	head->val = cpy;
}

void	handle_dollar_expansion(int *i, t_env_list *e,
		t_toknes_list *head, int flag3)
{
	t_expand_var	data;

	data.w = e;
	data.found = 0;
	data.expand = build_expand_string(i, head, &data.flag);
	data.e = e;
	data.flag3 = flag3;
	data.i = i;
	data.head = head;
	expand_helper(&data);
	if (!data.found)
	{
		if (data.expand[0] == '?')
		{
			head->flag_exit = 1;
			head->val = expand_val(ft_itoa(exit_status(0, 0)),
					data.head, *data.i, data.flag3);
		}
		else
			head->val = expand_val(ft_strdup(""),
					data.head, *data.i, data.flag3);
		if (data.flag)
			check_is_expandig(head, e, 0);
	}
}
