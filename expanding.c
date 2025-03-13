/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/13 01:40:00 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int check_is_expandig(t_toknes_list *head,char **env);
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
char* expand_val(char *s,char *s2,int j,int flag)
{
	int i;
	char *res;
	char *tmp;
	
	i = 0;
   
	res = ft_strdup("");
	printf("look1 %s\n",s2);
	while (s2[i] && s2[i] !='$')
	{
		tmp = res;
		res = join_character(res,s2[i]);
		i++;
		free(tmp);
	}
	i = 0;
	printf("look %s\n",res);
	while (s[i] && s[i] != '=')
		i++;
	i++;
	if(flag)
		res = join_character(res,'\"');
	while (s[i])
	{
		tmp = res;
		res = join_character(res,s[i]);
		i++;
		free(tmp);
	}
	if(flag)
		res = join_character(res,'\"');
	while (s2[j])
	{
		tmp = res;
		res = join_character(res,s2[j]);
		j++;
		free(tmp);
	}
	res = join_character(res,'\0');
	printf("final res: %s\n",res);
	return (res);
}
void remove_q_d(t_toknes_list *head)
{
	int i;
	char *cpy;
	char *tmp;
	
	i= 0;
	cpy = ft_strdup("");
	while (head->val[i])
	{
		if(head->val[i]=='\'')
		{
			i++;
			while (head->val[i] && head->val[i] !='\'')
			{
				tmp = cpy;
				cpy = join_character(cpy,head->val[i]);
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
				tmp = cpy;
				cpy = join_character(cpy,head->val[i]);
				i++;
			}
			if(head->val[i] =='\"')
				i++;
		}
		else
		{
			while (head->val[i] && head->val[i] !='\'' && head->val[i] !='\"')
			{
				tmp = cpy;
				cpy = join_character(cpy,head->val[i]);
				i++;
			}
		}
	}
	free(head->val);
	head->val = cpy;
}
static void handle_dollar_expansion(int *i,char **env,t_toknes_list *head,int flag3)
{
	char    *expand;
	char    *tmp;
	int     j;
	int flag;
	int flag2;

	flag2 = 0;
	flag = 0;
	j = 0;
	printf("%s\n",head->val);
	expand = ft_strdup("");
	while (head->val[*i]  &&!ft_isspace(head->val[*i]))
	{
		
		if(head->val[*i] == '$' || head->val[*i] =='\"' || head->val[*i] =='='  ||  head->val[*i] =='+')
		{
			flag = 1;
			break;
		}
		else if (head->val[*i] =='\'')
			break;
		else
		{	
			tmp = expand;
			expand = join_character(expand,head->val[*i]);
			
			free(tmp);
		}
		(*i)++;
	}
	printf("the expand : %s\n",expand);
	while (env && env[j])
	{
		if(strcmp_env(env[j],expand,ft_strlen(expand)))
		{
			flag2 = 1;
			tmp = head->val;
			head->val = expand_val(env[j],head->val,*i,flag3);
			if(flag)
				check_is_expandig(head,env);
			free(tmp);
		}  
		j++;
	}
	if(!flag2)
	{
		tmp = head->val;
		head->val = expand_val("",head->val,*i,flag3);
		if(flag)
			check_is_expandig(head,env);
		free(tmp);
	}
}

int check_is_expandig(t_toknes_list *head,char **env)
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
					handle_dollar_expansion(&i,env,head,0);
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
			if(head->val[i-1]!='=')
				head->split_it = 1;
			while (head->val[i] && head->val[i] =='$')
				i++;
			handle_dollar_expansion(&i,env,head,1);
		}
		i++;
	}
	return (i);
}

int check_expand(t_toknes_list *head,char **env)
{
	int i = 0;
	int flag = 0;
	while (head->val[i])
	{
		if (head->val[i] == '$')
		{
			i = check_is_expandig(head,env);
			flag = 1;
		}
		i++;
	}
	return (flag);
}

void expanding(t_toknes_list *token_head,char **env)
{
	t_toknes_list *head;

	head = token_head;
	while (head)
	{
		head->joined_str = ft_strdup(head->val);
		check_expand(head,env);
		remove_q_d(head);
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