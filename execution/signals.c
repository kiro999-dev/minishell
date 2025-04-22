/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:20:45 by onajem            #+#    #+#             */
/*   Updated: 2025/04/19 17:51:02 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler(int sig)
{
	printf("\n");
	if (sig == SIGINT)
	{
		exit_status(130, 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signals_handling(void)
{
	// signal(SIG, SIG_IGN);
	
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}

void	heredoc_handler(int sig)
{
	(void)sig;
	exit_herdoc(1, 1);
	exit_status(130, 1);
	ioctl(0, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	heredoc_signals(void)
{
	exit_herdoc(0, 1);
	signal(SIGINT, heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
