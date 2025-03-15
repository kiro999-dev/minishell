#include "../minishell.h"
#include "heper.h"

void sort_env(t_env_list **head)
{
    t_env_list *ptr;
    t_env_list *last;
    int swapped;
    if (!head || !(*head))
        return;
    last = NULL;
    swapped = 1;
    while (swapped)
    {
        swapped = 0;
        ptr = *head;
        while (ptr->next != last)
        {
            if (ft_strncmp(ptr->var, ptr->next->var, ft_strlen(ptr->var)) > 0)
            {
                char *temp = ptr->var;
                ptr->var = ptr->next->var;
                ptr->next->var = temp;
                swapped = 1;
            }
            ptr = ptr->next;
        }
        last = ptr;
    }
}


void print_export(t_env_list *e)
{
    if (!e)
    {
        printf("varibales not found!\n");
        return ;
    }
    while (e)
    {
        printf("declare -x %s\n", e->var);
        e = e->next;
    }
    
}

int replace_existing_key(t_env_list *env, char *key)
{
    t_env_list *current;
    
    current = env;
    while (current)
    {
        if (ft_strchr(key, '=') && equal_strcmp(current->var, key) == 0)
        {
            // free(current->var);
            current->var = strdup(key);
            return (1);
        }
        if (!ft_strchr(key, '=') && equal_strcmp(current->var, key) == 0)
            return (1);
        current = current->next;
    }
    return (0);
}


void add_var_2_env(char *cmd, t_env_list **env)
{
    t_env_list *new;

    if (replace_existing_key(*env, cmd))
        return;

    new = new_node(cmd);
    if (!new)
        return;
    if (!ft_strchr(cmd, '='))
        new->undec = 1;
    add_back(env, new);
}


t_env_list *copy_list(t_env_list *env)
{
    t_env_list *new_list;
    t_env_list *current;
    t_env_list *new;

    new_list = NULL;
    current = env;
    while (current)
    {
        new = new_node(ft_strdup(current->var));
        if (!new)
            return NULL;
        add_back(&new_list, new);
        current = current->next;
    }
    return new_list;
}

void f_export(char **cmd, t_env_list *ev)
{
    int i;
    t_env_list *tmp;

    if (!cmd)
        exit(1);
    if (size_2d(cmd) == 1)
    {
        tmp = copy_list(ev);
        sort_env(&tmp);
        print_export(tmp);
        free_env_list(tmp);
    }
    else
    {
        i = 1;
        while (cmd[i])
        {
            add_var_2_env(cmd[i], &ev);
            i++;    
        }
    }
}
