/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:11:58 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 16:16:23 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env_list	*lstlast(t_env_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	add_back(t_env_list **lst, t_env_list *new)
{
	t_env_list	*temp;

	if (!new || !lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	temp = lstlast(*lst);
	temp->next = new;
}

t_env_list	*new_node(void *content)
{
	t_env_list	*dest;

	if (!content)
		return (NULL);
	dest = (t_env_list *)gc_malloc(sizeof(t_env_list), 1);
	if (!dest)
		return (NULL);
	dest->var = content;
	dest->undec = 0;
	dest->next = (NULL);
	return (dest);
}

int	env_size(t_env_list *lst)
{
	int		i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
