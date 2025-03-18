#include "../minishell.h"
#include "heper.h"

void	*free_array(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (NULL);
}

int size_2d(char **arr)
{
    int i;

    if (!arr)
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

	dest = (t_env_list *)gc_malloc(sizeof(t_env_list));
	if (!dest)
		return (NULL);
	dest->var = content;
	dest->next = (NULL);
	return (dest);
}

void free_env_list(t_env_list *env)
{
    t_env_list *temp;
    
    while (env->next)
    {
        temp = env;
        env = env->next;
        free(temp->var);
        free(temp);
    }
}



t_env_list *init_env(char *ev[])
{
    int i = 0;
    t_env_list *env_list = NULL;
    t_env_list *new;

    if (!ev || !ev[0])
    {
        printf("env not found!\n");
        exit(1);
    }
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

    return env_list;
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