/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbge_collcter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:57:05 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/09 18:21:37 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*creat_value(int i, int flag, char *s)
{
	char	*res;

	res = ft_strdup("");
	if (s[i] == '\'')
		res = join_character(res, '\"');
	if (s[i] == '\"')
		res = join_character(res, '\'');
	else if (flag)
		res = join_character(res, '\"');
	while (s[i])
		res = join_character(res, s[i++]);
	if (i > 0 && s[i - 1] == '\"')
		res = join_character(res, '\'');
	if (i > 0 && s[i - 1] == '\'')
		res = join_character(res, '\"');
	else if (flag)
		res = join_character(res, '\"');
	return (res);
}

void	free_gc(t_gc_collector **gc_head)
{
	t_gc_collector	*tmp;

	while (*gc_head)
	{
		tmp = (*gc_head);
		(*gc_head) = (*gc_head)->next;
		free(tmp->ptr);
		tmp->ptr = NULL;
		free(tmp);
		tmp = NULL;
	}
	*gc_head = NULL;
}

void	*gc_malloc(size_t size, int flag)
{
	static t_gc_collector	*gc_head;
	t_gc_collector			*node;

	if (flag)
	{
		node = malloc(sizeof(t_gc_collector));
		if (node == NULL)
			return (free_gc(&gc_head),
				printf("Erorr malloc fail"), exit(1), NULL);
		node->ptr = malloc(size);
		if (node->ptr == NULL)
		{
			free(node);
			return (free_gc(&gc_head),
				printf("Erorr malloc fail"), exit(1), NULL);
		}
		node->next = gc_head;
		gc_head = node;
		return (node->ptr);
	}
	else
		free_gc(&gc_head);
	(void)size;
	return (NULL);
}
