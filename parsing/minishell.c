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



static int count_cmd_tokens(t_toknes_list *token)
{
	int count = 0;
	int flag = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD || token->type == CMD)
		{
			if (token->split_it)
			{
				if (token->val)
					count += counting_words(token->val, " \n\t");
				if (token->join_me)
					count--;
				flag = 1;
			}
			else if (!token->join_me && !flag)
				count++;
			else if (token->join_me && !flag)
				count++;
		}
		token = token->next;
		flag = 0;
	}
	return count;
}

int counting(char **spilt)
{
	int i = 0;
	if (!spilt)
		return (0);
	while (spilt[i])
		i++;
	return (i);
}

static char **realloc_cmd_array(char **cmd, int current_count, int extra)
{
	int new_size, i;
	char **new_cmd;
	i = 0;
	new_size = current_count + extra + 1;
	new_cmd = gc_malloc(sizeof(char *) * new_size);
	if (!new_cmd)
		return (NULL);
	while (i < current_count)
	{
		new_cmd[i] = cmd[i];
		i++;
	}
	new_cmd[current_count] = NULL;
	return (new_cmd);
}

static void process_split_it(t_toknes_list **current, char **cmd, int *i)
{
	int j = 0;
	char **split = ft_split((*current)->val, " \t\n");
	if ((*current)->join_me)
	{
		if (*i > 0)
		{
			(*i)--;
			cmd[*i] = ft_strjoin(cmd[*i], split[0]);
			(*i)++;
		}
		else
		{
			cmd[*i] = ft_strdup(split[0]);
			(*i)++;
		}
		j++;
	}
	while (split[j])
	{
		cmd[*i] = split[j];
		(*i)++;
		j++;
	}
	(*i)--;
	*current = (*current)->next;
	while (*current && (*current)->join_me && !(*current)->split_it)
	{
		if (*i < 0)
			*i = 0;
		cmd[*i] = ft_strjoin(cmd[*i], (*current)->val);
		*current = (*current)->next;
	}
	(*i)++;
}

static char **process_split_it2(t_toknes_list **current, char **cmd, int *i, int orig_count)
{
	int j = 0, re_count = 0;
	char **split = ft_split((*current)->val, " \t\n");
	re_count = counting(split);
	if ((*current)->join_me)
	{
		if (*i > 0)
		{
			(*i)--;
			cmd[*i] = ft_strjoin(cmd[*i], split[0]);
			(*i)++;
		}
		else
		{
			cmd[*i] = ft_strdup(split[0]);
			(*i)++;
		}
		j++;
	}
	cmd = realloc_cmd_array(cmd, orig_count, re_count);
	while (split[j])
	{
		cmd[*i] = split[j];
		(*i)++;
		j++;
	}
	(*i)--;
	*current = (*current)->next;
	while (*current && (*current)->join_me && !(*current)->split_it)
	{
		cmd[*i] = ft_strjoin(cmd[*i], (*current)->val);
		*current = (*current)->next;
	}
	(*i)++;
	return (cmd);
}

static void process_default(t_toknes_list **current, char **cmd, int *i)
{
	if ((*current)->val)
		cmd[*i] = ft_strdup((*current)->val);
	else
		cmd[*i] = ft_strdup("");
	*current = (*current)->next;
	while (*current && (*current)->join_me && !(*current)->split_it)
	{
		cmd[*i] = ft_strjoin(cmd[*i], (*current)->val);
		*current = (*current)->next;
	}
	(*i)++;
}

char **cmd_int(int count)
{
	char **cmd;
	int k = 0;
	cmd = gc_malloc(sizeof(char *) * (count + 1));
	while (k < count)
	{
		cmd[k] = NULL;
		k++;
	}
	return (cmd);
}

static char **copy_cmd_tokens(t_toknes_list *token, int count)
{
	t_toknes_list *current;
	char **cmd;
	int i = 0;
	cmd = cmd_int(count);
	current = token;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == CMD)
		{
			if (current->split_it)
				process_split_it(&current, cmd, &i);
			else if (current->split_it2 && cmd[0] && strcmp("export", cmd[0]))
				cmd = process_split_it2(&current, cmd, &i, count);
			else
				process_default(&current, cmd, &i);
		}
		else
			current = current->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

char **pars_cmd(t_toknes_list *token)
{
	int count = count_cmd_tokens(token);
	return (copy_cmd_tokens(token, count));
}

int isfile(t_TOKENS type)
{
	if (type == IS_FILE_IN || type == IS_FILE_OUT || type == IS_FILE_APPEND)
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
		else if (tokenz_head->type == HER_DOC)
		{
			tokenz_head = tokenz_head->next;
			if (tokenz_head)
			{
				node = creat_node_exc(NULL, LIMTER, f_head, ft_strdup(tokenz_head->val));
				add_back_list(exc_head, node);
				tokenz_head = tokenz_head->next;
			}
		}
		while (tokenz_head && (isfile(tokenz_head->type) || tokenz_head->type == REDIR_IN ||
			tokenz_head->type == REDIR_OUT || tokenz_head->type == APPEND))
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
	int i = 0;
	t_exc_lits *head = *exc_head;
	while (head)
	{
		i = 0;
		if (head && head->cmd)
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
		head = head->next;
	}
}

int main(int argc, char **argv, char **env)
{
	char *buff;
	t_toknes_list *head = NULL;
	t_exc_lits *exc_head = NULL;
	t_gc_collector *gc_head = NULL;
	argc = argc - 1;
	argv[0] = NULL;
	while (1)
	{
		buff = readline("> ");
		if (buff == NULL)
		{
			free_gc(&gc_head);
			break;
		}
		add_history(buff);
		lex(buff, &head);
		check_syntax(head);
		expanding(head, env);
		print_lits(head);
		generate_list(head, &exc_head);
		print_exc_list(&exc_head);
		free_gc(&gc_head);
		free(buff);
		buff = NULL;
		gc_head = NULL;
		exc_head = NULL;
		head = NULL;
	}
	return 0;
}
