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

void *gc_malloc(t_gc_collector **gc_head,size_t size)
{
	t_gc_collector *node;
	node = malloc(sizeof(t_gc_collector));
	if(node == NULL)
		return(NULL);
	node->ptr=malloc(size);
	if(node->ptr == NULL)
	{
		free(node);
		return (NULL);
	}
	node->next = *gc_head;
	*gc_head = node;
	return (node->ptr);
}
void free_gc(t_gc_collector **gc_head)
{
	t_gc_collector *tmp;
	int i = 0;
	while (*gc_head)
	{
		tmp = (*gc_head);
		(*gc_head)= (*gc_head)->next;
		free(tmp->ptr);
		tmp->ptr = NULL;
		free(tmp);
		tmp = NULL;
		i++;
	}
}
void free_list(t_toknes_list *head)
{
	t_toknes_list *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->val);
		;
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

static char **copy_cmd_tokens(t_toknes_list *token, int count,t_gc_collector **gc_head)
{
	t_toknes_list *current;
	char **cmd;
	int i;
	int j;
	char **split;
	

	j = 0;
	cmd = gc_malloc(gc_head,sizeof(char *) * (count + 1));
	cmd[0] = NULL;
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
				split = ft_split(current->val," \t\n",gc_head);

				if(current->join_me)
				{
					i--;
					cmd[i] = ft_strjoin(cmd[i],split[0],gc_head);
					printf("the res %s\n",cmd[i]);
					i++;
					j++;
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
					cmd[i] = ft_strjoin(cmd[i],current->val,gc_head);	
					current = current->next;
				}
				i++;
					
			}
			else if(current->split_it2 && cmd[0] && strcmp("export",cmd[0]))
			{
				j = 0;
				split = ft_split(current->val," \t\n",gc_head);
				if(current->join_me)
				{
					i--;
					cmd[i] = ft_strjoin(cmd[i],split[0],gc_head);
					printf("the res %s\n",cmd[i]);
					i++;
					j++;
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
					cmd[i] = ft_strjoin(cmd[i],current->val,gc_head);	
					current = current->next;
				}
				i++;
					
			}
			else 
			{
				cmd[i] = ft_strdup(current->val,gc_head);
				current = current->next;
				while (current && current->join_me && !current->split_it)
				{
					cmd[i] = ft_strjoin(cmd[i],current->val,gc_head);
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

char **pars_cmd(t_toknes_list *token,t_gc_collector **gc_head)
{

	int count = count_cmd_tokens(token);
	return( copy_cmd_tokens(token, count,gc_head));
}
int isfile(t_TOKENS type)
{
	if (type == IS_FILE_IN || type == IS_FILE_OUT 
			||type == IS_FILE_APPEND)
		return (1);
	return (0);
}

void generate_list(t_toknes_list *tokenz_head, t_exc_lits **exc_head,t_gc_collector **gc_head)
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
			cmd = pars_cmd(tokenz_head,gc_head);
			tokenz_head = tokenz_head->next;
		}
		else if(tokenz_head->type == HER_DOC)
		{
			tokenz_head = tokenz_head->next;
			if(tokenz_head)
			{
				node = creat_node_exc(NULL,LIMTER,f_head,ft_strdup(tokenz_head->val,gc_head),gc_head);
				add_back_list(exc_head,node);
				tokenz_head = tokenz_head->next;
			}
		}
		while (tokenz_head && (isfile(tokenz_head->type) || tokenz_head->type == REDIR_IN 
				||  tokenz_head->type == REDIR_OUT || tokenz_head->type == APPEND))
		{
			if (isfile(tokenz_head->type))
				add_list_file(&f_head, tokenz_head->val, tokenz_head->type,gc_head);
			tokenz_head = tokenz_head->next;
		}
		add_list_exc(exc_head, cmd, CMD, f_head,gc_head);
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

int main(int argc,char **argv,char **env)
{
	char *buff;
	t_toknes_list *head = NULL;
	t_exc_lits *exc_head = NULL;
	t_gc_collector *gc_head = NULL;
	argc = argc-1;
	argv[0] = NULL;
	while (1)
	{
		buff = readline("> ");
		if(buff == NULL)
		{
			free_gc(&gc_head);
			break;
		}
		add_history(buff);
		lex(buff, &head,&gc_head);
		check_syntax(head);
		expanding(head,env,&gc_head);
		print_lits(head);
		generate_list(head, &exc_head,&gc_head);
		print_exc_list(&exc_head);
		free_gc(&gc_head);
		free(buff);
		buff =NULL;
		gc_head = NULL;
		exc_head = NULL;
		head = NULL;
		buff = NULL;
	} 
	return 0;
}
