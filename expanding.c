/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/15 23:08:04 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int check_is_expandig(t_toknes_list *head,char **env,t_gc_collector **gc_head);
int not_character_expand(char c)
{
	if(!((c>='a' && c<='z') || (c>='A' && c<='Z') || c== '_'))
		return (1);
	return(0);
}
void skip_q_expand(char *s, int *ptr_i, int *q_ptr)
{
	int i = *ptr_i;
	int q = *q_ptr;
	
	while (q && s[i])
	{
		if (s[i] == '\'')
			q = !q;
		i++;
	}
	*ptr_i = i;
	*q_ptr = q;
}
int strcmp_env(char *s1,char *s2,int n)
{
	int i;
	
	i = 0;
	while (s1[i] && i < n)
	{
		if(s1[i] != s2[i])
			return(0);
		i++;
	}
	if(s1[i] != '=')
		return(0);
	return (1);
}
char* expand_val(char *s,char *s2,int j,int flag,int n,t_gc_collector **gc_head)
{
	int i;
	char *res;
	
	i = 0;
   
	res = ft_strdup("",gc_head);
	while (s2[i] && i < j-n-1 )
	{
		res = join_character(res,s2[i],gc_head);
		i++;
		
	}
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	if(s[i])
		i++;
	if(flag)
		res = join_character(res,'\"',gc_head);
	while (s[i])
	{
		res = join_character(res,s[i],gc_head);
		i++;
	}
	if(flag)
		res = join_character(res,'\"',gc_head);
	while (s2[j])
	{
		res = join_character(res,s2[j],gc_head);
		j++;
		
	}
	res = join_character(res,'\0',gc_head);
	return (res);
}
void remove_q_d(t_toknes_list *head,t_gc_collector **gc_head)
{
	int i;
	char *cpy;
	
	i= 0;
	cpy = ft_strdup("",gc_head);
	while (head->val[i])
	{
		if(head->val[i]=='\'')
		{
			i++;
			while (head->val[i] && head->val[i] !='\'')
			{
				cpy = join_character(cpy,head->val[i],gc_head);
				i++;
			}
			if(head->val[i] =='\'')
				i++;
		}
		else if(head->val[i]=='\"')
		{
			i++;
			while (head->val[i] && head->val[i] !='\"')
			{
				cpy = join_character(cpy,head->val[i],gc_head);
				i++;
			}
			if(head->val[i] =='\"')
				i++;
		}
		else
		{
			while (head->val[i] && head->val[i] !='\'' && head->val[i] !='\"')
			{
				cpy = join_character(cpy,head->val[i],gc_head);
				i++;
			}
		}
	}
	head->val = cpy;
}
static void handle_dollar_expansion(int *i,char **env,t_toknes_list *head,int flag3,t_gc_collector **gc_head)
{
	char    *expand;
	int     j;
	int flag;
	int flag2;

	flag2 = 0;
	flag = 0;
	j = 0;
	expand = ft_strdup("",gc_head);
	while (head->val[*i]  && !ft_isspace(head->val[*i]))
	{
		if(not_character_expand(head->val[*i]))
		{
			flag = 1;
			break;
		}
		else
			expand = join_character(expand,head->val[*i],gc_head);
		(*i)++;
	}
	
	while (env && env[j])
	{
		if(strcmp_env(env[j],expand,ft_strlen(expand)))
		{
			flag2 = 1;
			head->val = expand_val(env[j],head->val,*i,flag3,ft_strlen(expand),gc_head);
			if(flag)
				check_is_expandig(head,env,gc_head);
		}  
		j++;
	}
	if(!flag2)
	{
		head->val = expand_val("",head->val,*i,flag3,ft_strlen(expand),gc_head);
		if(flag)
			check_is_expandig(head,env,gc_head);
		
	}
}

int check_is_expandig(t_toknes_list *head,char **env,t_gc_collector **gc_head)
{
	int i = 0, dq = 0, q = 0;
	
	while (head->val[i])
	{
		if (head->val[i] == '\"')
		{
			dq = !dq;
			i++;
			if (head->val[i] == '\"')
				dq = !dq;
			while (dq && head->val[i])
			{
				if (head->val[i] == '$')
				{
					while (head->val[i] && head->val[i] =='$')
						i++;
					if(not_character_expand(head->val[i]))
						break;
					handle_dollar_expansion(&i,env,head,0,gc_head);
				}
				else if (head->val[i] == '\"')
					dq = !dq;
				else
					i++;
			}
		}
		else if (head->val[i] == '\'')
		{
			q = !q;
			skip_q_expand(head->val,&i, &q);
		}
		else if (head->val[i] == '$')
		{
			if(head->val[0]=='$' || head->val[0]=='=')
				head->split_it = 1;
			else
				head->split_it2 = 1;
			while (head->val[i] && head->val[i] =='$')
				i++;
			if(!not_character_expand(head->val[i]))
				handle_dollar_expansion(&i,env,head,1,gc_head);
		}
		i++;
	}
	return (i);
}

int check_expand(t_toknes_list *head,char **env,t_gc_collector **gc_head)
{
	int i = 0;
	int flag = 0;
	int len =ft_strlen(head->val) ;
	while (head && i < len )
	{
		if (head->val[i] == '$')
		{
			i = check_is_expandig(head,env,gc_head);
			flag = 1;
		}
		i++;
	}
	return (flag);
}

void expanding(t_toknes_list *token_head,char **env,t_gc_collector **gc_head)
{
	t_toknes_list *head;

	head = token_head;
	while (head)
	{
		check_expand(head,env,gc_head);
		remove_q_d(head,gc_head);
		head = head->next;
	}
}
//!(a-z A-Z _number)
//"ls -l"
//ls 	|-a 	| -l
//0  	1    	2
//creat word ls
// var="one two"
// echo "test"$var"hh""
// echo testone two