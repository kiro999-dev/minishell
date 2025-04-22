/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:04:45 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/18 14:36:55 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_toknes_list	*creat_node(char *val, t_TOKENS type, int join_me)
{
	t_toknes_list	*node;

	node = gc_malloc(sizeof(t_toknes_list), 1);
	if (!node)
		return (printf("malloc fail\n"), NULL);
	node->val = val;
	node->type = type;
	node->next = NULL;
	node->split_it = 0;
	node->join_me = join_me;
	node->split_it2 = 0;
	node->ambiguous = 0;
	node->len_expand = 0;
	node->flag_exit = 0;
	return (node);
}

t_list_here_doc	*creat_node_here_doc(char *limtter, t_TOKENS type, int here_doc)
{
	t_list_here_doc	*node;

	node = gc_malloc(sizeof(t_list_here_doc), 1);
	node->type = type;
	node->here_doc = here_doc;
	node->limtter = limtter;
	node->next = NULL;
	return (node);
}

void	add(t_toknes_list **head, char *val, t_TOKENS type, int join_me)
{
	t_toknes_list	*node;
	t_toknes_list	*tmp;

	if (head == NULL)
		return ;
	node = creat_node(val, type, join_me);
	if (!node)
		return ;
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

void	here_doc_add(t_list_here_doc **head, char *LIMTER, t_TOKENS type,
	int here_doc)
{
	t_list_here_doc	*node;
	t_list_here_doc	*tmp;

	if (head == NULL)
		return ;
	node = creat_node_here_doc(LIMTER, type, here_doc);
	if (!node)
		return ;
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
