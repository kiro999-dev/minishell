#include "../minishell.h"


int exit_status(int stat, int flag)
{
	static int e_status = 0;
	
	if (flag == 1)
		e_status = stat;
	return (e_status);
}


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
	}
}

void	signals_handling(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handler);
}


void heredoc_handler(int sig)
{
    (void)sig;
	exit_herdoc(1, 1);
	exit_status(130, )
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


void default_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

int check_exit(int status)
{
	int spec;

	spec = 0;
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        printf("\n");
        exit_status(128 + WTERMSIG(status), 1);
		spec = 1;
    }
    else if (WIFEXITED(status))
    {
        exit_status(WEXITSTATUS(status), 1);
    }
    signals_handling();
	return (spec);
}