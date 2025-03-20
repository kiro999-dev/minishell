/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 00:37:51 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/20 00:41:51 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *gen_double_q_str(int *i,char *s,t_tok *data,int q)
{
	char	*token;
	
	token = ft_strdup("");
	if(s[*i] == '\"')
	{
		q = !q;
        token = join_character(token,s[*i]);
		(*i)++;
	}
	while (s && s[*i])
	{
		if(s[*i] == '\"')
			q = !q;
		if(ft_isspace(s[*i]) && !q)
			break;
		token = join_character(token,s[*i]);
		if(s[*i] == '\"' && !data->is_redir_in && !data->is_redir_out && !data->is_append && !data->is_here_d)
		{
		 	(*i)++;
		 	break;
		}
	 	(*i)++;
	}
	return(token);
}
char *gen_single_q_str(int *i,char *s,t_tok *data,int q)
{
	char *token;
	
	token = ft_strdup("");
	if(s[*i] == '\'')
	{
		q = !q;
        token = join_character(token,s[*i]);
		(*i)++;
	}
	while (s && s[*i])
	{
		if(s[*i] == '\'')
			q = !q;
		if(ft_isspace(s[*i]) && !q)
			break;
		token = join_character(token,s[*i]);
		if(s[*i] == '\'' && !data->is_redir_in && !data->is_redir_out && !data->is_append && !data->is_here_d)
		{
		 	(*i)++;
		 	break;
		}
	 	(*i)++;
	}
	return(token);
}
void single_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head)
{
	char *token;
	
	token = gen_single_q_str(i_ptr,s,data,0);
	data->token = token;
	add_token(data,head);
}
void double_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head)
{
	char *token;
	
	token = gen_double_q_str(i_ptr,s,data,0);
	data->token = token;
	add_token(data,head);
}
