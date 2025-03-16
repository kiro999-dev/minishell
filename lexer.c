/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:48:22 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/16 19:33:17 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int ft_isspace(int c)
{
	if((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}
void add_token(t_tok *data_tok,t_toknes_list **head,t_gc_collector **gc_head);

static char *read_single_quote_token(char *s, int *i_ptr, t_tok *data, t_gc_collector **gc_head) 
{
	int i = *i_ptr, q = 0;
	char *token = ft_strdup("", gc_head);
	if (s[i] == '\'') {
		q = !q;
		token = join_character(token, s[i], gc_head);
		i++;
	}
	while (s[i]) 
	{
		if (s[i] == '\'')
			q = !q;
		if (ft_isspace(s[i]) && !q)
			break;
		token = join_character(token, s[i], gc_head);
		if (s[i] == '\'' && !data->is_redir_in && !data->is_redir_out &&
			!data->is_append && !data->is_here_d) {
			i++;
			break;
		}
		i++;
	}
	return (token);
}

static char *read_double_quote_token(char *s, int *i_ptr, t_tok *data, t_gc_collector **gc_head) {
	int i = *i_ptr, q = 0;
	char *token = ft_strdup("", gc_head);
	if (s[i] == '\"') 
	{
		q = !q;
		token = join_character(token, s[i], gc_head);
		i++;
	}
	while (s[i]) 
	{
		if (s[i] == '\"')
			q = !q;
		if (ft_isspace(s[i]) && !q)
			break;
		token = join_character(token, s[i], gc_head);
		if (s[i] == '\"' && !data->is_redir_in && !data->is_redir_out &&
			!data->is_append && !data->is_here_d)
			 {
			i++;
			break;
		}
		i++;
	}
	*i_ptr = i;
	return token;
}
void double_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head, t_gc_collector **gc_head) {
	char *token = read_double_quote_token(s, i_ptr, data, gc_head);
	if (token[0] == '\0')
		return;
	data->token = token;
	add_token(data, head, gc_head);
}
void single_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head, t_gc_collector **gc_head) 
{
	char *token = read_single_quote_token(s, i_ptr, data, gc_head);
	if (token[0] == '\0')
		return;
	data->token = token;
	add_token(data, head, gc_head);
}
int pipe_symbol(int *i_ptr,int *is_cmd,t_toknes_list **head,t_gc_collector **gc_head)
{
	int i;
	
	i = *i_ptr;
	add(head,ft_strdup("|",gc_head),PIPE,0,gc_head);
	*is_cmd = 1;
	*i_ptr = i;
	return(1);
}
int redir_in(int *i_ptr,char *s,t_toknes_list **head,t_tok *d,t_gc_collector **gc_head)
{
	int   i;

	i = *i_ptr;
	i++;

	if (s[i] == '<')
	{
		add(head,ft_strdup("<<",gc_head),HER_DOC,0,gc_head);
		d->is_here_d  = 1;
	}
	else
	{
      add(head,ft_strdup("<",gc_head),REDIR_IN,0,gc_head);
      d->is_redir_in = 1;
	  i--;
	}
	*i_ptr = i;
	return (1);
}
int redir_out(int *i_ptr,char *s,t_toknes_list **head,t_tok *d,t_gc_collector **gc_head)
{
	int   i;

	i = *i_ptr;
	i++;
	
	if (s[i] == '>')
	{
		add(head,ft_strdup(">>",gc_head),APPEND,0,gc_head);
		d->is_append = 1;
	}
	else
	{	
		add(head,ft_strdup(">",gc_head),REDIR_OUT,0,gc_head);
		d->is_redir_out = 1;
		i--;
	}
	*i_ptr = i;
	
	return (1);
}
void print(char *s , t_TOKENS type)
{
	
	if(s)
	{
			printf("%s--->",s);
	}
	if(type ==PIPE)
		printf("PIPE\n");
	if(type == CMD)
		printf("CMD\n");
	if(type == WORD)
		printf("WORD\n");
	if(type == REDIR_IN)
		printf("REDIR_IN\n");
	if(type == REDIR_OUT)
		printf("REDIR_OUT\n");
	if(type == APPEND)
		printf("APPEND\n");
	if(type == HER_DOC)
		printf("HER_DOC\n");
   if(type == IS_FILE_IN)
		printf("IS_FILE_IN\n");
	if(type == IS_FILE_OUT)
		printf("IS_FILE_OUT\n");
	if(type == LIMTER)
		printf("LIMTER\n");
	if(type == IS_FILE_APPEND)
		printf("IS_FILE_APPEND\n");
	if(s && type == DOLLAR)
		printf("the joind str\n");
	
}
void print_lits(t_toknes_list *head)
{
	while (head)
	{
		print(head->val,head->type);
		head = head->next;

	}
}

int is_not_token(char c)
{
	char *s ="<>|\'\"";
	if(ft_strchr(s,c) == NULL)
		return (0);
	else
		return (1);
}
void	init_tok(t_tok *data_tok)
{
	data_tok->is_append = 0;
	data_tok->is_cmd = 1;
	data_tok->is_here_d = 0;
	data_tok->is_redir_in = 0;
	data_tok->is_redir_out = 0;
	data_tok->q = 0;
	data_tok->dq = 0;
	data_tok->join_me = 1;
}
void token_add(t_toknes_list **head,t_tok *data_tok,t_TOKENS type,t_gc_collector **gc_head)
{
	
	if(type == CMD)
	{
		data_tok->is_cmd = 0;
		data_tok->join_me = 0;
	}
	add(head,ft_strdup(data_tok->token,gc_head),type,data_tok->join_me,gc_head);
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
void gen_word(t_tok  *data_tok, char *s,int *ptr_i,t_gc_collector **gc_head)
{
	int		i;

	i = *ptr_i;
	data_tok->token = ft_strdup("",gc_head);
	while (s[i] && !is_not_token(s[i]))
	{
		if(s[i] == '\'' && !data_tok->dq)
			data_tok->q = !data_tok->q;
		else if(s[i] == '\"' && !data_tok->q)
			data_tok->dq = !data_tok->dq;
	  	if(ft_isspace(s[i]) && !data_tok->q && !data_tok->dq)
			break;
	  	data_tok->token = join_character(data_tok->token,s[i],gc_head);
	   	;
	  	i++;
	}
	*ptr_i = i;
}
void add_token(t_tok *data_tok,t_toknes_list **head,t_gc_collector **gc_head)
{
	if (data_tok->is_cmd && !data_tok->is_redir_in 
		&& !data_tok->is_here_d  && !data_tok->is_redir_out)
		token_add(head,data_tok,CMD,gc_head);
	else if (data_tok->is_redir_in)
		token_add(head,data_tok,IS_FILE_IN,gc_head);
	else if (data_tok->is_redir_out)
		token_add(head,data_tok,IS_FILE_OUT,gc_head);
	else if (data_tok->is_append)
		token_add(head,data_tok,IS_FILE_APPEND,gc_head);
	else if(data_tok->is_here_d)
		token_add(head,data_tok,LIMTER,gc_head);
	else
		token_add(head,data_tok,WORD,gc_head);
}
void not_token_case(t_tok *data_tok,t_toknes_list **head,int *ptr_i,char *s,t_gc_collector **gc_head)
{
	gen_word(data_tok,s,ptr_i,gc_head);
	add_token(data_tok,head,gc_head);
}
void  lex(char *s, t_toknes_list **head,t_gc_collector **gc_head) // | '' "" cmd args << < > >>
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
		if(!is_not_token(s[i]) && s[i])
			not_token_case(&data_tok,head,&i,s,gc_head);
		else  if(s[i] == '|' &&  !pipe_symbol(&i,&data_tok.is_cmd,head,gc_head))
			return;
		else if(s[i] == '<' && !redir_in(&i,s,head,&data_tok,gc_head))
			return;
		else  if(s[i] == '>' && !redir_out(&i,s,head,&data_tok,gc_head))
			return;
		else  if(s[i] == '\'')
		{
			single_q(&i,s,&data_tok,head,gc_head);
			data_tok.join_me = 1;
		}
		else  if(s[i] == '\"')
		{
			double_q(&i,s,&data_tok,head,gc_head);
			data_tok.join_me = 1;
		}
		else 
			i++;
	}
}
