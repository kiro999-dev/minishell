/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:38:03 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 17:58:47 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!is_num(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	f_exit(char **cmd, t_data_parsing *data_exe, int child)
{
	int	e_status;

	e_status = 0;
	if (child != 1)
		printf("exit\n");
	if (cmd[1])
	{
		if (cmd[2])
		{
			write(2, "minishell : exit : too many arguments\n", 39);
			exit_status(1, 1);
			return ;
		}
		if (!is_numeric(cmd[1]))
		{
			write(2, "minishell : exit : numeric argument required\n", 46);
			e_status = 2;
		}
		else
			e_status = ft_atoi(cmd[1]) % 256;
	}
	free_gc(&data_exe->gc_head);
	exit(e_status);
}

void	print_export(t_env_list *e)
{
	if (!e)
	{
		return ;
	}
	while (e)
	{
		export_putstr(e->var);
		e = e->next;
	}	
}
