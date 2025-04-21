/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/21 20:17:24 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*build_prefix(char *s2, int j, int n)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	while (s2[i] && i < j - n - 1)
	{
		res = join_character(res, s2[i]);
		i++;
	}
	return (res);
}

char	*build_env_value(char *s, int flag,int flag_exit)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	if (!flag_exit)
	{
		while (s[i] && s[i] != '=')
			i++;
		if (s[i])
			i++;
	}
	if (s[i] =='\"' && flag)
		res = join_character(res, '\'');
	else if (flag)
		res = join_character(res, '\"');
	while (s[i])
	{
		res = join_character(res, s[i]);
		i++;
	}
	if(i > 0 && s[i-1] =='\"' && flag)
		res = join_character(res, '\'');
	else if (flag)
		res = join_character(res, '\"');
	return (res);
}

char	*build_suffix(char *s2, int j)
{
	char	*res;

	res = ft_strdup("");
	while (s2[j])
	{
		res = join_character(res, s2[j]);
		j++;
	}
	return (res);
}

char	*combine_parts(char *p, char *e, char *s)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (p[i])
	{
		res = join_character(res, p[i]);
		i++;
	}
	i = 0;
	while (e[i])
	{
		res = join_character(res, e[i]);
		i++;
	}
	i = 0;
	while (s[i])
	{
		res = join_character(res, s[i]);
		i++;
	}
	res = join_character(res, '\0');
	return (res);
}

char	*expand_val(char *s, t_toknes_list *head, int j, int flag)
{
	char	*p;
	char	*e;
	char	*suf;

	p = build_prefix(head->val, j, head->len_expand);
	e = build_env_value(s, flag, head->flag_exit);
	suf = build_suffix(head->val, j);
	return (combine_parts(p, e, suf));
}
