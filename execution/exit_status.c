/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:14:46 by onajem            #+#    #+#             */
/*   Updated: 2025/04/18 18:19:11 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_status(int stat, int flag)
{
	static int	e_status = 0;

	if (flag == 1)
		e_status = stat;
	return (e_status);
}

int	exit_herdoc(int x, int flag)
{
	static int	stat = 0;

	if (flag == 1)
		stat = x;
	return (stat);
}

int	check_exit(int status)
{
	int	spec;

	spec = 0;
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT)
			spec = 1;
		exit_status(128 + WTERMSIG(status), 1);
	}
	else if (WIFEXITED(status))
		exit_status(WEXITSTATUS(status), 1);
	signals_handling();
	return (spec);
}
