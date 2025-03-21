/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:04:45 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/22 08:19:36 by onajem           ###   ########.fr       */
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
