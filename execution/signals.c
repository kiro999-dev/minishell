#include "../minishell.h"

int g_status = 0;

void	handler(int sig)
{
	(void)sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_status = 1;
	}
}



void	signals_handling(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}

static void heredoc_handler(int sig)
{
    (void)sig;
    g_status = 1;
    write(STDOUT_FILENO, "\n", 1);
    close(STDIN_FILENO); // This will make readline() return NULL
}

void setup_heredoc_signals(void)
{
    signal(SIGINT, &heredoc_handler);
    signal(SIGQUIT, SIG_IGN);
}

void restore_default_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}