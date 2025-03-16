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
static void process_split_token(t_toknes_list **curr, char **cmd, int *i, t_gc_collector **gc_head) {
    char **split;
    int j = 0;
    split = ft_split((*curr)->val, " \t\n", gc_head);
    if ((*curr)->join_me && *i > 0) {
        *i -= 1;
        cmd[*i] = ft_strjoin(cmd[*i], split[0], gc_head);
        j++;
    }
    while (split[j])
        cmd[(*i)++] = split[j++];
    while ((*curr)->next && (*curr)->next->join_me && !(*curr)->next->split_it) {
        *curr = (*curr)->next;
        cmd[*i - 1] = ft_strjoin(cmd[*i - 1], (*curr)->val, gc_head);
    }
    *curr = (*curr)->next;
}

static void process_split_token2(t_toknes_list **curr, char **cmd, int *i, t_gc_collector **gc_head) {
    char **split;
    int j = 0;
    split = ft_split((*curr)->val, " \t\n", gc_head);
    if ((*curr)->join_me && *i > 0) {
        *i -= 1;
        cmd[*i] = ft_strjoin(cmd[*i], split[0], gc_head);
        j++;
    }
    while (split[j])
        cmd[(*i)++] = split[j++];
    while ((*curr)->next && (*curr)->next->join_me && !(*curr)->next->split_it) {
        *curr = (*curr)->next;
        cmd[*i - 1] = ft_strjoin(cmd[*i - 1], (*curr)->val, gc_head);
    }
    *curr = (*curr)->next;
}

static void process_regular_token(t_toknes_list **curr, char **cmd, int *i, t_gc_collector **gc_head) {
    cmd[(*i)++] = ft_strdup((*curr)->val, gc_head);
    while ((*curr)->next && (*curr)->next->join_me && !(*curr)->next->split_it) {
        *curr = (*curr)->next;
        cmd[*i - 1] = ft_strjoin(cmd[*i - 1], (*curr)->val, gc_head);
    }
    *curr = (*curr)->next;
}

static char **copy_cmd_tokens(t_toknes_list *token, int count, t_gc_collector **gc_head) {
    char **cmd;
    int i = 0;
    
    cmd = gc_malloc(gc_head, sizeof(char *) * (count + 1));
    if (!cmd)
        return NULL;
    while (token && token->type != PIPE) {
        if (token->type == WORD || token->type == CMD) 
	{
            if (token->split_it)
                process_split_token(&token, cmd, &i, gc_head);
            else if (token->split_it2 && i > 0 && strcmp("export", cmd[0]) != 0)
                process_split_token2(&token, cmd, &i, gc_head);
            else
                process_regular_token(&token, cmd, &i, gc_head);
        } else 
            token = token->next;
    }
    cmd[i] = NULL;
    return cmd;
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
static void process_cmd(t_toknes_list **tokenz, t_exc_lits **exc, t_gc_collector **gc_head) {
    char **cmd = NULL;
    t_file *f_head = NULL;
    if ((*tokenz)->type == CMD) {
        cmd = pars_cmd(*tokenz, gc_head);
        *tokenz = (*tokenz)->next;
    }
    while (*tokenz && (isfile((*tokenz)->type) || (*tokenz)->type == REDIR_IN ||
           (*tokenz)->type == REDIR_OUT || (*tokenz)->type == APPEND)) {
        if (isfile((*tokenz)->type))
            add_list_file(&f_head, (*tokenz)->val, (*tokenz)->type, gc_head);
        *tokenz = (*tokenz)->next;
    }
    add_list_exc(exc, cmd, CMD, f_head, gc_head);
}

static void process_heredoc(t_toknes_list **tokenz, t_exc_lits **exc, t_gc_collector **gc_head) {
    t_exc_lits *node = NULL;
    t_file *f_head = NULL;
    *tokenz = (*tokenz)->next;
    if (*tokenz) {
        node = creat_node_exc(NULL, LIMTER, f_head, ft_strdup((*tokenz)->val, gc_head), gc_head);
        add_back_list(exc, node);
        *tokenz = (*tokenz)->next;
    }
}

void generate_list(t_toknes_list *tokenz_head, t_exc_lits **exc_head, t_gc_collector **gc_head) {
    while (tokenz_head) {
        if (tokenz_head->type == CMD)
            process_cmd(&tokenz_head, exc_head, gc_head);
        else if (tokenz_head->type == HER_DOC)
            process_heredoc(&tokenz_head, exc_head, gc_head);
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
