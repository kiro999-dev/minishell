#include "../minishell.h"

int size_2d(char **arr)
{
    int i;

    if (!arr || !(*arr))
        return (0);
    i = 0;
    while (arr[i])
        i++;
    return (i);
}

t_env_list	*lstlast(t_env_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}


void add_back(t_env_list **lst, t_env_list *new)
{
    t_env_list *temp;

    if (!new || !lst)
        return;
    if (!(*lst))
    {
        *lst = new;
        return;
    }
    temp = lstlast(*lst);
    temp->next = new;
}

t_env_list	*new_node(void *content)
{
	t_env_list	*dest;

    if (!content)
        return (NULL);
    dest = (t_env_list *)gc_malloc(sizeof(t_env_list), 1);
	if (!dest)
		return (NULL);
	dest->var = content;
    dest->undec = 0;
	dest->next = (NULL);
	return (dest);
}

void handle_env_i(t_env_list **lst)
{
    t_env_list *new;

    new = new_node(ft_strdup("PWD=/home/onajem/Desktop/minishell"));
    add_back(lst, new);
    new = new_node(ft_strdup("SHLVL=1"));
    add_back(lst, new);
    new = new_node(ft_strdup("_=/usr/bin/env"));
    add_back(lst, new);    
}

t_env_list *init_env(char *ev[])
{
    int i = 0;
    t_env_list *env_list = NULL;
    t_env_list *new;

    if (!ev || !ev[0])
        handle_env_i(&env_list);
    else
    {
        while (ev[i])
        {
            new = new_node(ft_strdup(ev[i]));
            if (!new)
            {
                printf("Memory allocation failed!\n");
                exit(1);
            }
            add_back(&env_list, new);
            i++;
        }
    }

    return env_list;
}


int cmds_size(t_exc_lits *lst)
{
	int		i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
        lst = lst->next;
		i++;
	}
	return (i);
}

int	env_size(t_env_list *lst)
{
	int		i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
// int compare_key(char **array, char *key)
// {
//     int i;

//     if (!array || !key)
//         return (0);
//     i = -1;
//     while (array[++i])
//     {
//         if (equal_strcmp(array[i], key, 1) == 0)
//         {
//             free(array[i]);
//             array[i] = ft_strdup(key);
//             return (1);
//         }
//     }
//     return (0); 
// }