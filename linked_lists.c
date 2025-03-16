/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_lists.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:01:50 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/15 22:25:44 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exc_lits *creat_node_exc(char **cmd, t_TOKENS type, t_file *head_files,char *limiter,t_gc_collector **gc_head)
{
    t_exc_lits *node = gc_malloc(gc_head,sizeof(t_exc_lits));
    if (!node)
        return (printf("malloc fail\n"), NULL);
    node->cmd = cmd;
    node->type = type;
    node->next = NULL;
    node->head_files = head_files;
    if(limiter != NULL)
    {
        node->limiter = limiter;
        node->here_doc = 1;
    }
    else
        node->here_doc = 0;
    return node;
}
void add_back_list(t_exc_lits **head,t_exc_lits *node)
{
    t_exc_lits *tmp;
    
    if (head == NULL)
        return;
    if (!node)
        return;
    if (*head == NULL)
        *head = node;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}
void add_list_exc(t_exc_lits **head, char **cmd, t_TOKENS type, t_file *head_files,t_gc_collector **gc_head)
{
    t_exc_lits *node;
    t_exc_lits *tmp;
    
    if (head == NULL)
        return;
    node = creat_node_exc(cmd, type, head_files,NULL,gc_head);
    if (!node)
        return;
    if (*head == NULL)
        *head = node;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}


t_file *creat_node_file(char *name, t_TOKENS type,t_gc_collector **gc_head)
{
    t_file *node = gc_malloc(gc_head,sizeof(t_file));
    if (!node)
        return (printf("malloc fail\n"), NULL);
    node->type = type;
    node->file = name;
    node->next = NULL;
    return node;
}

void add_list_file(t_file **head, char *name, t_TOKENS type,t_gc_collector **gc_head)
{
    t_file *node;
    t_file *tmp;
    
    if (head == NULL)
        return;
    node = creat_node_file(name, type,gc_head);
    if (!node)
        return;
    if (*head == NULL)
        *head = node;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}
