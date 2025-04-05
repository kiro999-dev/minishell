#include "../minishell.h"

int g_status = 0;

void	handler(int sig)
{
	(void)sig;
	if (sig == SIGINT)
	{
		exit(1);
		// write(1, "\n", 1);
		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();
		// g_status = 1;
	}
}



void	signals_handling(void)
{
	// signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}