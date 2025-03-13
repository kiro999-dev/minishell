/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:48:22 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/13 21:45:56 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int ft_isspace(int c)
{
	if((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void single_q(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head)
{
	char *token;
	char *tmp;
	int i;
   
	i = *i_ptr;
	i++;
	token = ft_strdup("");
   while (s[i] && s[i] != '\'')
   {
        tmp = token;
        token = join_character(token,s[i]),free(tmp);;
        i++;
   }
	if(token[0] =='\0')
		return (free(token));
	if(*is_cmd)
	{
		add(head,ft_strdup(token),CMD);
		*is_cmd = 0;
	}
	else
		add(head,ft_strdup(token),WORD);
	free(token);
	i++;
	*i_ptr = i;
}

void double_q(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head)
{
	char *token;
	char *tmp;
	int i;
	
	i = *i_ptr;
	i++;
	token = ft_strdup("");
	while (s[i] && s[i] != '\"')
	{
	  tmp = token;
	  token = join_character(token,s[i]);
	  free(tmp);
	  i++;
	}
	if(token[0] =='\0')
		return (free(token));
	if(*is_cmd)
		add(head,ft_strdup(token),CMD);
	else
		add(head,ft_strdup(token),WORD);
	free(token);
	i++;
	*i_ptr = i;
	*is_cmd = 0;
}

int pipe_symbol(int *i_ptr,int *is_cmd,t_toknes_list **head)
{
	int i;
	
	i = *i_ptr;
	add(head,ft_strdup("|"),PIPE);
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
		add(head,ft_strdup("<<"),HER_DOC);
		d->is_here_d  = 1;
	}
	else
	{
      add(head,ft_strdup("<"),REDIR_IN);
      d->is_redir_in = 1;
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
		add(head,ft_strdup(">>"),APPEND);
		d->is_append = 1;
	}
	else
	{	
		add(head,ft_strdup(">"),REDIR_OUT);
		d->is_redir_out = 1;
	}
	*i_ptr = i;
	
	return (1);
}
void print(char *s , t_TOKENS type)
{
	if(s)
		printf("%s--->",s);
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
		print(head->joined_str,DOLLAR);
		head = head->next;
	}
}

int is_not_token(char c)
{
	char *s ="<>|";
	if(ft_strchr(s,c) == NULL)
		return (0);
	else
		return (1);
}
void	init_tok(t_tok *data_tok,int flag)
{
	data_tok->is_append = 0;
	if(flag)
		data_tok->is_cmd = 1;
	else
		data_tok->is_cmd = 0;
	data_tok->is_here_d = 0;
	data_tok->is_redir_in = 0;
	data_tok->is_redir_out = 0;
	data_tok->q = 0;
	data_tok->dq = 0;
}
void token_add(t_toknes_list **head,t_tok *data_tok,t_TOKENS type)
{
	add(head,ft_strdup(data_tok->token),type);
	free(data_tok->token);
	if(type == CMD)
		data_tok->is_cmd = 0;
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
	char	*tmp;
	int		i;

	i = *ptr_i;
	data_tok->token = ft_strdup("");
	while (s[i] && !is_not_token(s[i]))
	{
		if(s[i] == '\'' && !data_tok->dq)
			data_tok->q = !data_tok->q;
		else if(s[i] == '\"' && !data_tok->q)
			data_tok->dq = !data_tok->dq;
	  	if(ft_isspace(s[i]) && !data_tok->q && !data_tok->dq)
			break;
	  	tmp = data_tok->token;
	  	data_tok->token = join_character(data_tok->token,s[i]);
	   	free(tmp);
	  	i++;
	}
	*ptr_i = i;
}
void not_token_case(t_tok *data_tok,t_toknes_list **head,int *ptr_i,char *s)
{
	gen_word(data_tok,s,ptr_i);
	
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
void  lex(char *s, t_toknes_list **head,int flag) // | '' "" cmd args << < > >>
{
	t_tok 	data_tok;
	int		i;
	int 	len;

	init_tok(&data_tok,flag);
	i = 0;
	len = ft_strlen(s);
	while (i < len && s[i])
	{
		while (i < len && ft_isspace(s[i]))
			i++;
		if(!is_not_token(s[i]) && s[i])
			not_token_case(&data_tok,head,&i,s);
		else  if(s[i] == '|' &&  !pipe_symbol(&i,&data_tok.is_cmd,head))
			return;
		else if(s[i] == '<' && !redir_in(&i,s,head,&data_tok))
			return;
		else  if(s[i] == '>' && !redir_out(&i,s,head,&data_tok))
			return;
		else 
			i++;
	}
}
