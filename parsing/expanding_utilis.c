/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utilis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/19 00:34:58 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	split_t2condi(char *s, int i)
{
	while (s[i])
	{
		if (s[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	not_character_expand(char c)
{
	if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
			|| c == '_' || (c >= '0' && c <= '9')))
		return (1);
	return (0);
}

void	skip_q_expand(char *s, int *ptr_i, int *q_ptr)
{
	int	i;
	int	q;

	i = *ptr_i;
	q = *q_ptr;
	i++;
	while (q && s[i])
	{
		if (s[i] == '\'')
			q = !q;
		i++;
	}
	*ptr_i = i;
	*q_ptr = q;
}

int	strcmp_env(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	while (s1[i] && i < n)
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	if (s1[i] != '=')
		return (0);
	return (1);
}

void	expand_helper(t_expand_var *data)
{
	while (data->w)
	{
		data->head->flag_exit = 0;
		data->head->len_expand = ft_strlen(data->expand);
		if (strcmp_env(data->w->var, data->expand, ft_strlen(data->expand)))
		{
			data->found = 1;
			data->head->val = expand_val(data->w->var,
					data->head, *data->i, data->flag);
			(*data->i) -= data->head->len_expand;
			break ;
		}
		data->w = data->w->next;
	}
}
