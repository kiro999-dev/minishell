/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:51:49 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 21:27:04 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void single_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head)
{
	char *token;
	int i;
	int q;

	q = 0;
	i = *i_ptr;
	token = ft_strdup("");
	if(s[i] == '\'')
	{
		q = !q;
        token = join_character(token,s[i]);
		i++;
	}
	while (s[i])
   	{
		if(s[i] == '\'')
			q = !q;
		if(ft_isspace(s[i]) && !q)
			break;
        token = join_character(token,s[i]);
		if(s[i] == '\'' && !data->is_redir_in && !data->is_redir_out && !data->is_append && !data->is_here_d)
		{
			i++;
			break;
		}
        i++;
   	}
	if(token[0] =='\0')
	{
		*i_ptr = i;
		return;
	}
	data->token = token;
	add_token(data,head);
	*i_ptr = i;
}
void double_q(int *i_ptr,char *s,t_tok *data,t_toknes_list **head)
{
	char *token;
	
	int i;
	int q;

	q = 0;
	i = *i_ptr;
	token = ft_strdup("");
	if(s[i] == '\"')
	{
		q = !q;
        token = join_character(token,s[i]);
		i++;
	}
	while (s[i])
   	{
		if(s[i] == '\"')
			q = !q;
		if(ft_isspace(s[i]) && !q)
			break;
        token = join_character(token,s[i]);
		if(s[i] == '\"' && !data->is_redir_in && !data->is_redir_out && !data->is_append && !data->is_here_d)
		{
			i++;
			break;
		}
        i++;
   	}
	if(token[0] =='\0')
	{
		*i_ptr = i;
		return;
	}
	data->token = token;
	add_token(data,head);
	*i_ptr = i;
}

int pipe_symbol(int *i_ptr,int *is_cmd,t_toknes_list **head)
{
	int i;
	
	i = *i_ptr;
	add(head,ft_strdup("|"),PIPE,0);
	*is_cmd = 1;
	*i_ptr = i;
	return(1);
}
int redir_in(int *i_ptr,char *s,t_toknes_list **head,t_tok *d)
{
	int   i;

	i = *i_ptr;
	i++;

	if (s[i] == '<')
	{
		add(head,ft_strdup("<<"),HER_DOC,0);
		d->is_here_d  = 1;
	}
	else
	{
      add(head,ft_strdup("<"),REDIR_IN,0);
      d->is_redir_in = 1;
	  i--;
	}
	*i_ptr = i;
	return (1);
}
int redir_out(int *i_ptr,char *s,t_toknes_list **head,t_tok *d)
{
	int   i;

	i = *i_ptr;
	i++;
	
	if (s[i] == '>')
	{
		add(head,ft_strdup(">>"),APPEND,0);
		d->is_append = 1;
	}
	else
	{	
		add(head,ft_strdup(">"),REDIR_OUT,0);
		d->is_redir_out = 1;
		i--;
	}
	*i_ptr = i;
	
	return (1);
}