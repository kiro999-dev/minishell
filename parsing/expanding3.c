/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:38:26 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 21:38:40 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *build_expand_string(int *i, t_toknes_list *head, int *flag) {
    char *expand = ft_strdup("");
    *flag = 0;
    while (head->val[*i] && !ft_isspace(head->val[*i])) {
        if (not_character_expand(head->val[*i])) {
            *flag = 1;
            break;
        }
        expand = join_character(expand, head->val[*i]);
        (*i)++;
    }
    return expand;
}

static void process_quote(char *s, int *i, char **cpy, char quote) {
    (*i)++;
    while (s[*i] && s[*i] != quote) {
        *cpy = join_character(*cpy, s[*i]);
        (*i)++;
    }
    if (s[*i] == quote)
        (*i)++;
}

static char *process_unquoted(char *s, int *i) {
    char *cpy = ft_strdup("");
    while (s[*i] && s[*i] != '\'' && s[*i] != '\"') {
        cpy = join_character(cpy, s[*i]);
        (*i)++;
    }
    return cpy;
}
void remove_q_d(t_toknes_list *head) 
{
    int i = 0;
    char *cpy = ft_strdup("");
    while (head->val[i]) {
        if (head->val[i] == '\'')
            process_quote(head->val, &i, &cpy, '\'');
        else if (head->val[i] == '\"')
            process_quote(head->val, &i, &cpy, '\"');
        else {
            char *temp = process_unquoted(head->val, &i);
            int j = 0;
            while (temp[j])
                cpy = join_character(cpy, temp[j++]);
        }
    }
    head->val = cpy;
}

void handle_dollar_expansion(int *i, t_env_list  *e, t_toknes_list *head, int flag3)
 {
    int flag;
    char *expand = build_expand_string(i, head, &flag);
    
    int found = 0;
    while (e) {
        if (strcmp_env(e->var, expand, ft_strlen(expand))) {
            found = 1;
            head->val = expand_val(e->var, head->val, *i, flag3, ft_strlen(expand));
            if (flag)
                check_is_expandig(head, e);
            break;
        }
        e = e->next;
    }
    if (!found) {
        head->val = expand_val("", head->val, *i, flag3, ft_strlen(expand));
        if (flag)
            check_is_expandig(head, e);
    }
}

