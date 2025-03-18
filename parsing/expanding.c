/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: onajem <onajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 16:05:08 by onajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int not_character_expand(char c) {
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'))
        return 1;
    return 0;
}

void skip_q_expand(char *s, int *ptr_i, int *q_ptr) {
    int i = *ptr_i, q = *q_ptr;
    
    i++;
    printf("this -->%c",s[i]);
    while (q && s[i]) 
    {
        if (s[i] == '\'')
            q = !q;
        i++;
    }
    *ptr_i = i;
    *q_ptr = q;
}

int strcmp_env(char *s1, char *s2, int n) {
    int i = 0;
    while (s1[i] && i < n) {
        if (s1[i] != s2[i])
            return 0;
        i++;
    }
    if (s1[i] != '=')
        return 0;
    return 1;
}

static char *build_prefix(char *s2, int j, int n) {
    int i = 0;
    char *res = ft_strdup("");
    while (s2[i] && i < j - n - 1) {
        res = join_character(res, s2[i]);
        i++;
    }
    return res;
}

static char *build_env_value(char *s, int flag) {
    int i = 0;
    char *res = ft_strdup("");
    while (s[i] && s[i] != '=')
        i++;
    if (s[i])
        i++;
    if (flag)
        res = join_character(res, '\"');
    while (s[i]) {
        res = join_character(res, s[i]);
        i++;
    }
    if (flag)
        res = join_character(res, '\"');
    return res;
}

static char *build_suffix(char *s2, int j) {
    char *res = ft_strdup("");
    while (s2[j]) {
        res = join_character(res, s2[j]);
        j++;
    }
    return res;
}

static char *combine_parts(char *p, char *e, char *s) {
    char *res = ft_strdup("");
    int i = 0;
    while (p[i]) {
	    res = join_character(res, p[i]); i++; }
    i = 0;
    while (e[i]) { res = join_character(res, e[i]); i++; }
    i = 0;
    while (s[i]) { res = join_character(res, s[i]); i++; }
    res = join_character(res, '\0');
    return res;
}

char* expand_val(char *s, char *s2, int j, int flag, int n) {
    char *p = build_prefix(s2, j, n);
    char *e = build_env_value(s, flag);
    char *suf = build_suffix(s2, j);
    return combine_parts(p, e, suf);
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

void remove_q_d(t_toknes_list *head) {
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

static void handle_dollar_expansion(int *i, t_env_list  *e, t_toknes_list *head, int flag3) {
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

static void expand_in_double_quotes(t_toknes_list *head, t_env_list  *e, int *i) {
    int dq = 1;
    (*i)++;
    if (head->val[*i] == '\"')
        dq = 0;
    while (dq && *i < ft_strlen(head->val) && head->val[*i]) {
        if (head->val[*i] == '$') {
            while (head->val[*i] && head->val[*i] == '$')
                (*i)++;
            if (not_character_expand(head->val[*i]))
                break;
            handle_dollar_expansion(i, e, head, 0);
        } else if (head->val[*i] == '\"')
            dq = 0;
        else
            (*i)++;
    }
}

static void expand_plain(t_toknes_list *head, t_env_list  *e, int *i) {
    if (head->val[0] == '$' || head->val[0] == '=')
        head->split_it = 1;
    else
        head->split_it2 = 1;
    while (head->val[*i] && head->val[*i] == '$')
        (*i)++;
    if (!not_character_expand(head->val[*i]))
        handle_dollar_expansion(i, e, head, 1);
}

int check_is_expandig(t_toknes_list *head, t_env_list  *e) {
    int i = 0, q = 0;
    while (i < ft_strlen(head->val) &&  head->val[i]) {
        if (head->val[i] == '\"')
            expand_in_double_quotes(head, e, &i);
        else if (head->val[i] == '\'') {
            q = 1;
            skip_q_expand(head->val, &i, &q);
        } else if (head->val[i] == '$')
            expand_plain(head, e, &i);
        i++;
    }
    return i;
}

int check_expand(t_toknes_list *head, t_env_list *e) {
    
    int i = 0;
    int flag = 0; 
    int len;

    len = ft_strlen(head->val);
    while (head && head->val && i < len) 
    {
        if (head->val[i] == '$') 
        {
            i = check_is_expandig(head, e);
            flag = 1;
        }
        len = ft_strlen(head->val);
        i++;
    }
    return flag;
}

void expanding(t_toknes_list *token_head, t_env_list  *e) {
    t_toknes_list *head = token_head;
    while (head) {
        check_expand(head, e);
        remove_q_d(head);
        head = head->next;
    }
}
