#include "../minishell.h"

int g_status = 0;

int set_herdoc_delimeter(int exit, int flag)
{
	static int x = 0;

	if (flag == 1)
		x = exit;
	return (x);
}


void	handler(int sig)
{
	(void)sig;
	printf("\n");
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = SIGINT;
	}
}

void	signals_handling(void)
{
	set_herdoc_delimeter(0, 1);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}



void	handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		set_herdoc_delimeter(1, 1);
		// g_status = 130;      
		write(1, "\n", 1);
		close(0);
	}
}

void heredoc_signals(void)
{
    signal(SIGINT, handler_heredoc);
    signal(SIGQUIT, SIG_IGN);
}
