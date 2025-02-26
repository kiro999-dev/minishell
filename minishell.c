/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:37:53 by zkhourba          #+#    #+#             */
/*   Updated: 2025/02/26 13:50:27 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_toknes_list *creat_node(char *val,t_TOKENS type)
{
	t_toknes_list *node;
	node = malloc(sizeof(t_toknes_list));
	if(!node)
		return (printf("malloc fail\n"),NULL);
	node->val = val;
	node->type = type;
	node->next = NULL;
	return (node);
}

void add(t_toknes_list **head,char *val,t_TOKENS type)
{
		t_toknes_list *node;
		t_toknes_list *tmp;
		
		if(head == NULL)
			return ;
		node = creat_node(val,type);
		if (!node)
			return ;
		if(*head == NULL)
			*head = node;
		else
		{
			tmp = *head;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = node;
		}
		
}
void print(char *s , t_TOKENS type)
{
	printf("%s--->",s);
	if(type ==PIPE)
		printf("PIPE\n");
	if(type == CMD)
		printf("CMD\n");
	if(type == ARG)
		printf("ARG\n");
	if(type == STRING)
		printf("STRING\n");
	if(type == REDIR_IN)
		printf("REDIR_IN\n");
	if(type == REDIR_OUT)
		printf("REDIR_OUT\n");
	if(type == APPEND)
		printf("APPEND\n");
	if(type == HER_DOC)
		printf("HERE_doc\n");
}
void print_lits(t_toknes_list *head)
{
	while (head)
	{
		print(head->val,head->type);
		head = head->next;
	}
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
	  token = join_character(token,s[i]);
	  free(tmp);
	  i++;
	}
	if(*is_cmd)
	{
		add(head,ft_strdup(token),CMD);
		*is_cmd = 0;
	}
	else
		add(head,ft_strdup(token),STRING);
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
	if(*is_cmd)
		add(head,ft_strdup(token),CMD);
	else
		add(head,ft_strdup(token),STRING);
	free(token);
	i++;
	*i_ptr = i;
	*is_cmd = 0;
}

int pipe_symbol(int *i_ptr,char *s,int *is_cmd,t_toknes_list **head)
{
	int i;
	
	i = *i_ptr;
	i++;
	if(s[i] == '|')
	{
		printf("syntax erorr hh\n");
		return (0);
	}
	add(head,ft_strdup("|"),PIPE);
	*is_cmd = 1;
	*i_ptr = i;
	return(1);
}
int redir_in(int *i_ptr,char *s,t_toknes_list **head)
{
	int   i;

	i = *i_ptr;
	i++;
	if (s[i] == '<' && s[i + 1] == '<')
	{
		printf("syntax erorr hh\n");
		return (0);
	}
	else if (s[i] == '<')
	{
		add(head,ft_strdup("<<"),HER_DOC);
		i++;
	}
	else
		add(head,ft_strdup("<"),REDIR_IN);
	*i_ptr = i;
	return (1);
}
int redir_out(int *i_ptr,char *s,t_toknes_list **head)
{
	int   i;

	i = *i_ptr;
	i++;
	if (s[i] == '>' && s[i + 1] == '>')
	{
		printf("syntax erorr hh\n");
		i++;
		return (0);
	}
	else if (s[i] == '>')
	{
		add(head,ft_strdup(">>"),APPEND);
		i++;
	}
	else
		add(head,ft_strdup(">"),REDIR_OUT);
	*i_ptr = i;
	return (1);
}
int is_not_token(char c)
{
	char *s ="<>\'\"| ";
	if(ft_strchr(s,c) == NULL)
		return (0);
	else
		return (1);
}
void  lex(char *s, t_toknes_list **head) // | '' "" cmd args << < > >>
{
	int   i;
	char  *token;
	char  *tmp;
	int   is_cmd;
	
	i = 0;
	is_cmd = 1;
	while (s[i])
	{
		while (isspace(s[i]) && s[i])
			i++;
		if(s[i] == '\'')
			single_q(&i,s,&is_cmd,head);
		else if(s[i] == '\"')
			double_q(&i,s,&is_cmd,head);
		else  if(s[i] == '|' &&  !pipe_symbol(&i,s,&is_cmd,head))
			return;
		else if(s[i] == '<' && !redir_in(&i,s,head))
			return;
		else  if(s[i] == '>' && !redir_out(&i,s,head))
			return;
		else if(!is_not_token(s[i]))
		{
			token = ft_strdup("");
			while (!is_not_token(s[i]) && s[i])
			{
				tmp = token;
				token = join_character(token,s[i]);
				free(tmp);
				i++;
			}
			if(is_cmd)
			{
				add(head,ft_strdup(token),CMD);
				free(token);
				is_cmd = 0;
			}
			else
			 {
				add(head,ft_strdup(token),ARG);
				free(token);
			 }
		}
		else 
		{
			printf("i skip -%c-\n",s[i]);
			i++;
		}
	}
}

int main()
{
	char *buff = "echo hello | ls /user/bin > file1 << > >> ";
	t_toknes_list *head = NULL;
	lex(buff,&head);
	print_lits(head);

	return (0);
}
