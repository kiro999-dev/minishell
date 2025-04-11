#include "../minishell.h"

int cmd_in_out_redirection(t_exc_lits *cmd, int red)
{
    t_file *file = cmd->head_files;
    while (file)
    {
        if (red == 0 && file->type == IS_FILE_IN)
            return (1);
        if (red && (file->type == IS_FILE_APPEND || file->type == IS_FILE_OUT))
            return (1);
        file = file->next;
    }
    return (0);
}

void apply_input_redirection(int *last_input_fd, const char *file)
{
    if (*last_input_fd != -1)
        close(*last_input_fd);

    *last_input_fd = open(file, O_RDONLY);
    if (*last_input_fd == -1)
        exit(1) ;
}

void apply_output_redirection(int *last_output_fd, const char *file, t_TOKENS type)
{
    if (*last_output_fd != -1)
        close(*last_output_fd);

    if (type == IS_FILE_OUT )  
        *last_output_fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    else if(type == IS_FILE_APPEND)
        *last_output_fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);

    if (*last_output_fd == -1)
        exit(1) ;
}


void set_final_redirections(int last_input_fd, int last_output_fd)
{
    if (last_input_fd != -1)
    {
        dup2(last_input_fd, STDIN_FILENO);
        close(last_input_fd);
    }
    
    if (last_output_fd != -1)
    {
        dup2(last_output_fd, STDOUT_FILENO);
        close(last_output_fd);
    }
}

char *generate_random_filename(void)
{
    int         random_fd;
    unsigned    random_value;
    char        *filename;
    char        random_str[9];

    random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1)
        return (NULL);
    
    if (read(random_fd, &random_value, sizeof(random_value)) != sizeof(random_value))
    {
        close(random_fd);
        return (NULL);
    }
    close(random_fd);

    hexa_format(random_value, random_str);

    filename = ft_strjoin("/tmp/minishell_heredoc_", random_str);
    return (filename);
}