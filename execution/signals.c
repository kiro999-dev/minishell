#include "../minishell.h"

int g_status = 0;

int exit_herdoc(int x, int flag)
{
	static int stat = 0;

	if (flag == 1)
		stat = x;
	return (stat);
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
	int tty_fd;
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd != -1)
		dup2(tty_fd, 0);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}


void heredoc_handler(int sig)
{
    (void)sig;
	exit_herdoc(1, 1);
	ioctl(0, TIOCSTI, "\n");
    rl_replace_line("", 0);
    rl_on_new_line();          

}

void heredoc_signals(void)
{
	exit_herdoc(0, 1);
    signal(SIGINT, heredoc_handler);
    signal(SIGQUIT, SIG_IGN);
}
