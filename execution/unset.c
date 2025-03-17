#include "heper.h"
#include "../minishell.h"


void pop_node(t_env_list **env, t_env_list *to_pop, t_env_list *prev)
{
    if (!to_pop)
        return ;
    if (prev)
        prev->next = to_pop->next;
    else 
        *env = to_pop->next;
    free(to_pop->var);
    free(to_pop);
}

t_env_list *find_env_node(t_env_list *env, char *key, t_env_list **prev)
{
    t_env_list *current = env;
    *prev = NULL;

    current = env;
    while (current)
    {
        if (!equal_strcmp(current->var, key))
            return (current);
        *prev = current;
        current = current->next;
    }
    return (NULL);
}

void f_unset(t_env_list **env, char **var)
{
    t_env_list *prev;
    t_env_list *target;
    int i;

    if (!env || !var)
    {
        printf("something went wrong !\n");
        return ;
    }
    i = -1;
    while (var[++i])
    {
        target = find_env_node(*env, var[i], &prev);
        if (target)
            pop_node(env, target, prev);
    }
}