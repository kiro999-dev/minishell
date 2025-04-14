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
    new = new_node(ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"));
    add_back(lst, new);  
}

char *handle_shlvl(char *lvl)
{
    char *equal_sign;
    char *value_str;
    int value;
    char *new_value_str;
    char *result;

    equal_sign = ft_strchr(lvl, '=');
    if (!equal_sign || !*(equal_sign + 1))
        return ft_strdup("SHLVL=1");
    value_str = equal_sign + 1;
    value = ft_atoi(value_str);
    value++;
    if (value > 9998)
    {
        printf("bash: warning: shell level %d too high, resetting to 1\n", value);
        value = 1;
    }
    new_value_str = ft_itoa(value);
    result = ft_strjoin("SHLVL=", new_value_str);

    return result;
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
            if (equal_strcmp(ev[i], "SHLVL") == 0)
                new = new_node(handle_shlvl(ev[i]));
            else 
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