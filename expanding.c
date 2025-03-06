/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/04 20:00:59 by zkhourba         ###   ########.fr       */
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

static void handle_dollar_expansion(const char *s, int *i)
{
    while (s[*i] && s[*i] != '\"' && s[*i] != '\'')
    {
        printf("%c", s[*i]);
        (*i)++;
    }
    printf("\nexpanded\n");
}

void check_is_expandig(char *s)
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
                    handle_dollar_expansion(s, &i);
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
            handle_dollar_expansion(s, &i);
        }
        i++;
    }
}

void check_expand(char *s)
{
    int i = 0;
    while (s[i])
    {
        if (s[i] == '$')
            check_is_expandig(s);
        i++;
    }
}

void expanding(t_toknes_list *head)
{
    while (head)
    {
        check_expand(head->val);
        head = head->next;
    }
}