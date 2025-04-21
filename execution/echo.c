/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:09:34 by onajem            #+#    #+#             */
/*   Updated: 2025/04/21 20:41:44 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo_putstr(char *s)
{
	if (!s)
		return ;
	write(1, s, ft_strlen(s));
}

int	echo_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	if (str[0] == '-' && !str[1])
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	f_echo(char **cmd)
{
	int	i;
	int	nwl;
	int	has_content;

	i = 1;
	nwl = 0;
	has_content = 0;
	while (cmd[i] && echo_flag(cmd[i]))
	{
		nwl = 1;
		i++;
	}
	while (cmd[i])
	{
		if (has_content)
			write(1, " ", 1);
		echo_putstr(cmd[i]);
		has_content = 1;
		i++;
	}
	if (!nwl)
		write(1, "\n", 1);
	exit_status(0, 1);
}
