/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:04:45 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/14 11:09:14 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_toknes_list *creat_node(char *val, t_TOKENS type,int join_me)
{
    t_toknes_list *node = malloc(sizeof(t_toknes_list));
    if (!node)
        return (printf("malloc fail\n"), NULL);
    node->val = val;
    node->type = type;
    node->next = NULL;
    node->split_it = 0;
    node->join_me = join_me;
    node->split_it2 = 0;
    return node;
}

void add(t_toknes_list **head, char *val, t_TOKENS type,int join_me)
{
    t_toknes_list *node;
    t_toknes_list *tmp;
    
    if (head == NULL)
        return;
    node = creat_node(val, type,join_me);
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
        node->prv = tmp;
    }
}