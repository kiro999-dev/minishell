#include "../minishell.h"

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
        return ;
    }
    while (e)
    {
        export_putstr(e->var);
        e = e->next;
    }
    
}


char *trim_plus_sign(char *key)
{
    char *equal_pos;
    char *new_key;
    int len_before_plus;
    int total_len;

    equal_pos = custom_strnstr(key, "+=", strlen(key));
    if (!equal_pos)
        return key;
    len_before_plus = equal_pos - key; 
    total_len = strlen(key) - 1;     
    new_key = gc_malloc(total_len + 1,1);
    if (!new_key)
        return NULL;
    memcpy(new_key, key, len_before_plus); 
    memcpy(new_key + len_before_plus, equal_pos + 1, strlen(equal_pos)); 
    new_key[total_len] = '\0'; 
    // free(key);
    return new_key;
}


int replace_existing_key(t_env_list *env, char *key)
{
    t_env_list *current;
    char    *declared;
    
    current = env;
    if (custom_strnstr(key, "+=", ft_strlen(key)))
        return (-1);
    
    declared = ft_strchr(key, '=');
    while (current)
    {
        if (declared && equal_strcmp(current->var, key) == 0)
        {
            // free(current->var);
            current->var = ft_strdup(key);
            return (1);
        }
        if (!declared && equal_strcmp(current->var, key) == 0)
            return (1);
        current = current->next;
    }
    return (0);
}

int valid_key(char *var)
{
	int		i;
	int		key_len;
	char	*equal;

	if (!var || !var[0] || (!(ft_isalpha(var[0]) || var[0] == '_')))
		return (0);
	equal = ft_strchr(var, '=');
	if (equal)
		key_len = equal - var;
	else
		key_len = ft_strlen(var);
	if (key_len > 1 && var[key_len - 1] == '+')
		key_len--;
	i = 1;
	while (i < key_len)
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}


void get_key_value(char *var, char **key, char **value)
{
    char *plus_pos;

    plus_pos = custom_strnstr(var, "+=", ft_strlen(var));
    if (!plus_pos)
    return ;
    *key = ft_substr(var, 0, plus_pos - var);
    if (!(*key))
        return ;
    *value = ft_strdup(plus_pos + 2);
    if (!(*value))
    {
        return ;
    }
}


int check_append(t_env_list *env, char *var)
{
    char *key;
    char *value;
    char *tmp;

    key = NULL;
    value = NULL;
    get_key_value(var, &key, &value);
    
    if (!key || !value)
        return (0);
    while (env)
    {
        if (!equal_strcmp(key, env->var))
        {
            if (!ft_strchr(env->var, '='))
                value = ft_strjoin("=", value);
            tmp = ft_strjoin(env->var, value);
            env->var = tmp;

            return (1);
        }
        env = env->next;
    }
    return (0);
}

void add_var_2_env(char *cmd, t_env_list **env)
{
    t_env_list *new;
    int if_key_exist;

    if (check_append(*env, cmd))
        return ;
    if_key_exist = replace_existing_key(*env, cmd);
    if (if_key_exist == 1)
        return;
    else if (if_key_exist == -1)
        cmd = trim_plus_sign(cmd);
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

void f_export(char **cmd, t_env_list **ev)
{
    int i;
    t_env_list *tmp;

    if (!cmd)
        exit(1);
    i = -1;
    if (size_2d(cmd) == 1)
    {
        tmp = copy_list(*ev);
        sort_env(&tmp);
        print_export(tmp);
    }
    else
    {
        i = 1;
        while (cmd[i])
        {
            if (!valid_key(cmd[i]))
                printf("minishell: export: `%s': not a valid identifier\n", cmd[i]);
            else
                add_var_2_env(cmd[i], ev);
            i++;    
        }
    }
}

