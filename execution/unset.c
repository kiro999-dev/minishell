/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:30:34 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 18:33:22 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pop_node(t_env_list **env, t_env_list *to_pop, t_env_list *prev)
{
	if (!to_pop)
		return ;
	if (!equal_strcmp(to_pop->var, "_"))
		return ;
	if (prev)
		prev->next = to_pop->next;
	else
		*env = to_pop->next;
}

t_env_list	*find_env_node(t_env_list *env, char *key, t_env_list **prev)
{
	t_env_list	*current;

	*prev = NULL;
	current = env;
	while (current)
	{
		if (!equal_strcmp(current->var, key))
			return (current);
		*prev = current;
		current = current->next;
	}
	return (NULL);
}

void	f_unset(t_env_list **env, char **var)
{
	t_env_list	*prev;
	t_env_list	*target;
	int			i;

	if (!env || !var)
	{
		printf("something went wrong !\n");
		return ;
	}
	i = -1;
	while (var[++i])
	{
		target = find_env_node(*env, var[i], &prev);
		if (target)
			pop_node(env, target, prev);
	}
	exit_status(0, 1);
}
