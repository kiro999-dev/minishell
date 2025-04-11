
#include "../minishell.h"


void	hexa_format(unsigned int value, char *output)
{
	const char	*hex_digits;
	int			i;

	i = 0;
    hex_digits = "0123456789abcdef";
	while (i < 8)
	{
		output[i] = hex_digits[(value >> (28 - i * 4)) & 0xF];
		i++;
	}
	output[8] = '\0';
}

int process_heredocs(t_exc_lits *cmd,t_env_list *e)
{
    t_list_here_doc *herdoc_head;
    int fd;
    char *line;
    char *filename;
    int i;

    i = 0;
    heredoc_signals();
    while (cmd && exit_herdoc(0, 0) != 1)
    {
        filename = generate_random_filename();
        herdoc_head = cmd->head_here_doc;
        while (herdoc_head && exit_herdoc(0, 0) != 1)
        {
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                // need to be handled !! it hang up
                break;
            }
            while (1)
            {
                line = readline("> ");
                if (exit_herdoc(0, 0))
                {
                    close(fd);
                    free(line);
                    unlink(filename);
                    break;
                }
                if (!line || !ft_strcmp(line, herdoc_head->limtter))
                {
                    free(line);
                    break;
                }
                if(check_expand_h(&line,e))
                {
                    write(fd, line, ft_strlen(line));
                    write(fd, "\n", 1);
                }
                else
                {
                    write(fd, line, ft_strlen(line));
                    write(fd, "\n", 1);
                    free(line);
                }
            }
            close(fd);
            cmd->heredoc_filename = filename; 
            herdoc_head = herdoc_head->next;
        }
        cmd = cmd->next;
        i++;
    }
    if (exit_herdoc(0, 0) == 1)
        i = 0;
    else 
        i = 1; 
    signals_handling();
    return (i); 
}