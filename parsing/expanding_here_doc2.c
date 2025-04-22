/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_here_doc2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:43:04 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/19 00:56:22 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*build_expand_string_h(int *i, char *val, int *flag)
{
	char	*expand;

	expand = ft_strdup("");
	*flag = 0;
	while (val[*i])
	{
		if(val[*i] == '?')
		{
			*flag = 1;
			(*i)++;
			return(ft_strdup("?"));
		}
		else if (not_character_expand(val[*i]) && (val[*i] != '\"' 
			&& val[*i] != '\''))
		{
			*flag = 1;
			break ;
		}
		else if(not_character_expand(val[*i]))
			break;
		expand = join_character(expand, val[*i]);
		(*i)++;
	}
	return (expand);
}

int	handle_dollar_expansion_h2(int *i, t_env_list *e,
char **val, char *v)
{
	int		flag;
	char	*expand;
	int		found;

	found = 0;
	expand = build_expand_string_h(i, v, &flag);
	while (e)
	{
		if (strcmp_env(e->var, expand, ft_strlen(expand)))
		{
			found = 1;
			*val = expand_val_h(e->var, v, *i, ft_strlen(expand));
			if (flag)
				check_is_expandig_h(val, e, 1);
			break ;
		}
		e = e->next;
	}
	return (found);
}

int	handle_dollar_expansion_h(int *i, t_env_list *e,
	char **val, int check)
{
	int		flag;
	char	*expand;
	int		found;
	char	*v;
	int		j;

	j = *i;
	expand = build_expand_string_h(&j, *val, &flag);
	v = ft_strdup(*val);
	if (check == 0)
	{
		free(*val);
		*val = NULL;
	}
	found = handle_dollar_expansion_h2(i, e, val, v);
	if (!found)
	{
		if(expand[0] == '?')
		{
			*val = expand_val_status_h(ft_itoa(exit_status(0,0)), v, *i, ft_strlen(expand));
			(*i)++;
		}
		else
			*val = expand_val_h(ft_strdup(""), v, *i, ft_strlen(expand));
		if (flag)
			check_is_expandig_h(val, e, 1);
	}
	return (1);
}

char	*expand_val_h(char *s, char *val, int j, int len_expand)
{
	char	*p;
	char	*e;
	char	*suf;
	int		flag;

	flag = 0;
	if(s[0] == '?')
		flag = 1;
	p = build_prefix(val, j, len_expand);
	e = build_env_value(s, 0,flag);
	suf = build_suffix(val, j);
	return (combine_parts(p, e, suf));
}
char	*expand_val_status_h(char *s, char *val, int j, int len_expand)
{
	char	*p;
	char	*e;
	char	*suf;


	p = build_prefix(val, j, len_expand);
	e = build_env_value(s, 0,1);
	suf = build_suffix(val, j);
	return (combine_parts(p, e, suf));
}