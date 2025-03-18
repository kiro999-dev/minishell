#include "../minishell.h"

void execution(t_data_parsing *data_exec)
{

    if (data_exec->head_exe->cmd)
    {
        if (!ft_strncmp(data_exec->head_exe->cmd[0], "export", 7))
        {
            printf("here\n");
            f_export(data_exec->head_exe->cmd, data_exec->e);
            print_export(data_exec->e);
        }
        if (!ft_strncmp(data_exec->head_exe->cmd[0], "env", 4))
            f_env(data_exec->e);
    
        if (!ft_strncmp(data_exec->head_exe->cmd[0], "unset", 6))
        {
            f_unset(&data_exec->e, data_exec->head_exe->cmd);
            print_export(data_exec->e);
        }
    
        if (!ft_strncmp(data_exec->head_exe->cmd[0], "cd", 3))
            f_cd(data_exec->head_exe->cmd[1], data_exec->e);
        
        if ( !ft_strncmp(data_exec->head_exe->cmd[0], "echo", 5))  
            f_echo(data_exec->head_exe->cmd);
    
        if (!ft_strncmp(data_exec->head_exe->cmd[0], "pwd", 4))  
            f_pwd();
    }

}