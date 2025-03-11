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
t_env init_env(char *ev[])
{
    int i;
    t_env env;

    env.size = 0;
    if (!ev)
    {
        printf("env not found!");
        exit(1);
    }
    env.size = size_2d(ev);
    env.env_arr = (char**) malloc ((env.size * sizeof(char*)) + 1);
    env.undeclared = (char**) malloc (sizeof(char*));
    if (!env.env_arr || !env.undeclared)
        exit(1);
    env.env_arr[env.size] = NULL;
    env.undeclared[0] = NULL;
    env.size_undec = 0;
    i = -1;
    while (++i < env.size)
    {
        env.env_arr[i] = ft_strdup(ev[i]);
        if (!env.env_arr[i])
            (free_array(env.env_arr), exit(1));
    }
    return (env);
}

int compare_key(char **array, char *key)
{
    int i;

    if (!array || !key)
        return (0);
    i = -1;
    while (array[++i])
    {
        if (equal_strcmp(array[i], key) == 0)
        {
            free(array[i]);
            array[i] = ft_strdup(key);
            return (1);
        }
    }
    return (0); 
}