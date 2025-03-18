/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:58:53 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 17:22:42 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_q(char *s)
{
    int i = 0, q = 0, dq = 0;
    
    while (s[i])
    {
        if (s[i] == '\'' && !dq)
            q = !q;
        if (s[i] == '\"' && !q)
            dq = !dq;
        i++;
    }
    if (dq)
        return (printf("syntax error\n"), 1);
    if (q)
        return (printf("syntax error\n"), 1);
    return 0;
}

int check_pipe(t_toknes_list *head)
{
    if (!head->next || !head->prv || head->next->type == PIPE)
        return (printf("syntax error near pipe\n"), 1);
    return 0;
}

int check_redir(t_toknes_list *head)
{
    if (!head->next || (head->next->type != IS_FILE_IN &&
                         head->next->type != IS_FILE_APPEND &&
                         head->next->type != IS_FILE_OUT))
        return (printf("syntax error '%s'\n", head->val), 1);
    return 0;
}

int check_here_doc(t_toknes_list *head)
{
    if (!head->next || head->next->type != LIMTER)
        return (printf("syntax error near '%s'\n", head->val), 1);
    return 0;
}

int check_syntax(t_toknes_list *head)
{
    while (head)
    {
        if (check_q(head->val))
            return (1);
        if (head->type == PIPE && check_pipe(head))
            return (1);
        if ((head->type == REDIR_IN || head->type == REDIR_OUT) &&
            check_redir(head))
            return (1);
        if (head->type == HER_DOC && check_here_doc(head))
            return (1);
        head = head->next;
    }
    return(0);
}
 