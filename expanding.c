/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/07 15:35:52 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void skip_q_expand(char *s, int *ptr_i, int *q_ptr)
{
    int i = *ptr_i;
    int q = *q_ptr;
    
    while (q && s[i])
    {
        if (s[i] == '\'')
            q = !q;
        i++;
    }
    *ptr_i = i;
    *q_ptr = q;
}
int strcmp_env(char *s1,char *s2,int n)
{
    int i;
    
    i = 0;
    while (s1[i] && i < n)
    {
        if(s1[i] != s2[i])
            return(0);
        i++;
    }
    if(s1[i] != '=')
        return(0);
    return (1);
}
char* expand_val(char *s,char *s2,int j)
{
    int i;
    char *res;
    char *tmp;
    
    i = 0;
    
    while (s[i] && s[i] != '=')
        i++;
    res = ft_strdup(s+i+1);
    while (s2[j])
    {
        tmp = res;
        res = join_character(res,s2[j]);
        j++;
        free(tmp);
    }
    printf("final res: %s\n",res);
    return (res);
}
static void handle_dollar_expansion(const char *s, int *i,char **env,t_toknes_list *head)
{
    char    *expand;
    char    *tmp;
    int     j;
    
    j = 0;
    (*i)++;
    expand = ft_strdup("");
    while (s[*i] && s[*i] != '\"' && s[*i] != '\'')
    {
        tmp = expand;
       expand = join_character(expand,s[*i]);
        (*i)++;
        free(tmp);
    }

    printf("%s\n",expand);
    while (env && env[j])
    {
        if(strcmp_env(env[j],expand,ft_strlen(expand)))
        {
            tmp = head->val;
            head->val = expand_val(env[j],head->val,*i);
        }
        j++;
    }
}

void check_is_expandig(char *s,t_toknes_list *head,char **env)
{
    int i = 0, dq = 0, q = 0;
    
    while (s[i])
    {
        if (s[i] == '\"')
        {
            dq = !dq;
            i++;
            if (s[i] == '\"')
                dq = !dq;
            while (dq && s[i])
            {
                if (s[i] == '$')
                    handle_dollar_expansion(s, &i,env,head);
                else
                {
                    if (s[i] == '\"')
                        dq = !dq;
                    i++;
                }
            }
        }
        else if (s[i] == '\'')
        {
            q = !q;
            skip_q_expand(s, &i, &q);
        }
        else if (s[i] == '$')
        {
            handle_dollar_expansion(s, &i,env,head);
        }
        i++;
    }
}

void check_expand(char *s,t_toknes_list *head,char **env)
{
    int i = 0;
    while (s[i])
    {
        if (s[i] == '$')
            check_is_expandig(s,head,env);
        i++;
    }
}

void expanding(t_toknes_list *token_head,char **env)
{
    t_toknes_list *head;

    head = token_head;
    while (head)
    {
        check_expand(head->val,head,env);
        head = head->next;
    }
}