/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Updated: 2025/03/04 05:08:57 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void free_list(t_toknes_list *head)
{
	t_toknes_list *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->val);
		free(tmp);
	}
}

static int count_cmd_tokens(t_toknes_list *token)
{
	int count = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD || token->type == CMD)
		{
			if(token->split_it)
			{
				count += counting_words(token->val," \n\t");
				if(token->join_me)
					count--;
			}
			else if(!token->join_me)
			{
				count++;
			}
		}
		token = token->next;
	}
	return count;
}

static char **copy_cmd_tokens(t_toknes_list *token, int count)
{
	t_toknes_list *current;
	char **cmd;
	int i;
	int j;
	char **split;
	char *tmp;

	j = 0;
	cmd = malloc(sizeof(char *) * (count + 1));
	i = 0;
	current = token;
	if (!cmd)
		return NULL;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == CMD)
		{
			if(current->split_it)
			{
				j = 0;
				split = ft_split(current->val," \t\n");
				printf("split me %d %s\n",current->split_it,current->val);
				if(current->join_me)
				{
					i--;
					tmp = cmd[i];
					cmd[i] = ft_strjoin(cmd[i],split[0]);
					printf("the res %s\n",cmd[i]);
					i++;
					j++;
					free(tmp);
				}
				while (split[j])
				{
					cmd[i] = split[j];
					i++;
					j++;
				}
				i--;
				current = current->next;
				while (current && current->join_me && !current->split_it)
				{
					printf("split me %d %s\n",current->split_it,current->val);
					tmp = cmd[i];
					cmd[i] = ft_strjoin(cmd[i],current->val);	
					free(tmp);
					current = current->next;
				}
				i++;
					
			}
			else if(current->split_it2 && strcmp("export",cmd[0]))
			{
				j = 0;
				split = ft_split(current->val," \t\n");
				printf("split me %d %s\n",current->split_it,current->val);
				if(current->join_me)
				{
					i--;
					tmp = cmd[i];
					cmd[i] = ft_strjoin(cmd[i],split[0]);
					printf("the res %s\n",cmd[i]);
					i++;
					j++;
					free(tmp);
				}
				while (split[j])
				{
					cmd[i] = split[j];
					i++;
					j++;
				}
				i--;
				current = current->next;
				while (current && current->join_me && !current->split_it)
				{
					printf("split me %d %s\n",current->split_it,current->val);
					tmp = cmd[i];
					cmd[i] = ft_strjoin(cmd[i],current->val);	
					free(tmp);
					current = current->next;
				}
				i++;
					
			}
			else 
			{
				cmd[i] = ft_strdup(current->val);
				current = current->next;
				while (current && current->join_me && !current->split_it)
				{
					tmp = cmd[i];
					cmd[i] = ft_strjoin(cmd[i],current->val);	
					free(tmp);
					current = current->next;
				}
				i++;
			}
		}
		else
			current = current->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

char **pars_cmd(t_toknes_list *token)
{
	t_toknes_list *tmp;

	tmp = token;
	int count = count_cmd_tokens(token);
	return( copy_cmd_tokens(token, count));
}
int isfile(t_TOKENS type)
{
	if (type == IS_FILE_IN || type == IS_FILE_OUT 
			||type == IS_FILE_APPEND)
		return (1);
	return (0);
}

void generate_list(t_toknes_list *tokenz_head, t_exc_lits **exc_head)
{
	t_exc_lits *node;
	t_file *f_head; 
	char **cmd;

	while (tokenz_head)
	{
		cmd = NULL;
		f_head = NULL;
		node = NULL;
		if (tokenz_head->type == CMD)
		{
			cmd = pars_cmd(tokenz_head);
			tokenz_head = tokenz_head->next;
		}
		else if(tokenz_head->type == HER_DOC)
		{
			tokenz_head = tokenz_head->next;
			if(tokenz_head)
			{
				node = creat_node_exc(NULL,LIMTER,f_head,ft_strdup(tokenz_head->val));
				add_back_list(exc_head,node);
				tokenz_head = tokenz_head->next;
			}
		}
		while (tokenz_head && (isfile(tokenz_head->type) || tokenz_head->type == REDIR_IN 
				||  tokenz_head->type == REDIR_OUT || tokenz_head->type == APPEND))
		{
			if (isfile(tokenz_head->type))
				add_list_file(&f_head, tokenz_head->val, tokenz_head->type);
			tokenz_head = tokenz_head->next;
		}
		add_list_exc(exc_head, cmd, CMD, f_head);
		if (tokenz_head && (tokenz_head->type == PIPE || tokenz_head->type == WORD))
			tokenz_head = tokenz_head->next;
	}
}

void print_list_file(t_file *head)
{
	while (head)
	{
		print(head->file, head->type);
		head = head->next;
	}
}

void print_exc_list(t_exc_lits **exc_head)
{
	int i;
	i = 0;
	t_exc_lits *head;

	head = *exc_head;
	while (head)
	{
		i = 0;
		if ( head &&head->cmd)
		{
			printf("[");
			while (head->cmd[i])
			{
				printf("-%s- ", head->cmd[i]);
				i++;
			}
			printf("]\n");
		}
		if (head->head_files)
		{
			printf("\nstart :\n");
			print_list_file(head->head_files);
			printf("end\n");
		}
		// if(head->limiter)
		//     printf("limiter of here_doc :%s\n",head->limiter);
		head = head->next;
	}
}
void join_token(t_toknes_list *head,t_toknes_list **final_lits)
{
	char *tmp;
	char *str;
	t_TOKENS type;

	while (head)
	{
		str = head->val;
		type = head->type;
		while (head && head->next && head->next->join_me)
		{
			tmp = head->val;
			str = ft_strjoin(str,head->next->val);
			free(tmp);
			head = head->next;
		}
		add(final_lits,str,type,0);
		head = head->next;
	}
}
int main(int argc,char **argv,char **env)
{
	char *buff;
	t_toknes_list *head = NULL;
	t_exc_lits *exc_head = NULL;
	argc = argc-1;
	argv[0] = NULL;
	while (1)
	{
		buff = readline("> ");
		if(buff == NULL)
			break;
		add_history(buff);
		lex(buff, &head,1); // handle file when its 'o''u''t' end every_case
		check_syntax(head);
		expanding(head,env);
		print_lits(head);
		generate_list(head, &exc_head);
		print_exc_list(&exc_head);
		exc_head = NULL;
		// free_list(head);
		head = NULL;
		// free(buff);
		buff = NULL;
	} 
	return 0;
}
