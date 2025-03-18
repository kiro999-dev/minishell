/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:48:23 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 21:27:13 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
int is_token(char c)
{
	char *s ="<>|\'\"";
	if(ft_strchr(s,c) == NULL)
		return (0);
	else
		return (1);
}
void token_add(t_toknes_list **head,t_tok *data_tok,t_TOKENS type)
{
	if(type == CMD)
	{
		data_tok->is_cmd = 0;
		data_tok->join_me = 0;
	}
	add(head,ft_strdup(data_tok->token),type,data_tok->join_me);
	data_tok->join_me = 1;
	if(type == IS_FILE_IN)
		data_tok->is_redir_in = 0;
	if(type == IS_FILE_OUT)
		data_tok->is_redir_out = 0;
	if(type == LIMTER)
		data_tok->is_here_d = 0;
	if(type == IS_FILE_APPEND)
		data_tok->is_append = 0;
}

void gen_word(t_tok  *data_tok, char *s,int *ptr_i)
{
	int		i;

	i = *ptr_i;
	data_tok->token = ft_strdup("");
	while (s[i])
	{
		if(s[i] == '\'' && !data_tok->dq)
			data_tok->q = !data_tok->q;
		else if(s[i] == '\"' && !data_tok->q)
			data_tok->dq = !data_tok->dq;
	  	if(ft_isspace(s[i]) && !data_tok->q && !data_tok->dq)
			break;
		if(is_token(s[i]) && (s[i] == '\'' || s[i] == '\"') && isparam(data_tok))
	  		data_tok->token = join_character(data_tok->token,s[i]);
		else if(is_token(s[i]))
	  		break;
		else
			data_tok->token = join_character(data_tok->token,s[i]);
	  	i++;
	}
	*ptr_i = i;
}
void add_token(t_tok *data_tok,t_toknes_list **head)
{
	if (data_tok->is_cmd && !data_tok->is_redir_in 
		&& !data_tok->is_here_d  && !data_tok->is_redir_out)
		token_add(head,data_tok,CMD);
	else if (data_tok->is_redir_in)
		token_add(head,data_tok,IS_FILE_IN);
	else if (data_tok->is_redir_out)
		token_add(head,data_tok,IS_FILE_OUT);
	else if (data_tok->is_append)
		token_add(head,data_tok,IS_FILE_APPEND);
	else if(data_tok->is_here_d)
		token_add(head,data_tok,LIMTER);
	else
		token_add(head,data_tok,WORD);
}
void  lex(char *s, t_toknes_list **head) 
{
	t_tok 	data_tok;
	int		i;
	int 	len;

	init_tok(&data_tok);
	i = 0;
	len = ft_strlen(s);
	while (i < len && s[i])
	{
		while (i < len && ft_isspace(s[i]))
		{
			i++;
			data_tok.join_me = 0;
		}
		if(!is_token(s[i]) && s[i])
			not_token_case(&data_tok,head,&i,s);
		else  if(s[i] == '|' &&  !pipe_symbol(&i,&data_tok.is_cmd,head))
			return;
		else if(s[i] == '<' && !redir_in(&i,s,head,&data_tok))
			return;
		else  if(s[i] == '>' && !redir_out(&i,s,head,&data_tok))
			return;
		else  if(s[i] == '\'')
		{
			single_q(&i,s,&data_tok,head);
			data_tok.join_me = 1;
		}
		else  if(s[i] == '\"')
		{
			double_q(&i,s,&data_tok,head);
			data_tok.join_me = 1;
		}
		else 
			i++;
	}
}
