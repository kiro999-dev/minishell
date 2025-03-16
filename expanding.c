/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/15 23:08:04 by zkhourba         ###   ########.fr       */
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
    while (q && s[i]) {
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

static char *build_prefix(char *s2, int j, int n, t_gc_collector **gc_head) {
    int i = 0;
    char *res = ft_strdup("", gc_head);
    while (s2[i] && i < j - n - 1) {
        res = join_character(res, s2[i], gc_head);
        i++;
    }
    return res;
}

static char *build_env_value(char *s, int flag, t_gc_collector **gc_head) {
    int i = 0;
    char *res = ft_strdup("", gc_head);
    while (s[i] && s[i] != '=')
        i++;
    if (s[i])
        i++;
    if (flag)
        res = join_character(res, '\"', gc_head);
    while (s[i]) {
        res = join_character(res, s[i], gc_head);
        i++;
    }
    if (flag)
        res = join_character(res, '\"', gc_head);
    return res;
}

static char *build_suffix(char *s2, int j, t_gc_collector **gc_head) {
    char *res = ft_strdup("", gc_head);
    while (s2[j]) {
        res = join_character(res, s2[j], gc_head);
        j++;
    }
    return res;
}

static char *combine_parts(char *p, char *e, char *s, t_gc_collector **gc_head) {
    char *res = ft_strdup("", gc_head);
    int i = 0;
    while (p[i]) {
	    res = join_character(res, p[i], gc_head); i++; }
    i = 0;
    while (e[i]) { res = join_character(res, e[i], gc_head); i++; }
    i = 0;
    while (s[i]) { res = join_character(res, s[i], gc_head); i++; }
    res = join_character(res, '\0', gc_head);
    return res;
}

char* expand_val(char *s, char *s2, int j, int flag, int n, t_gc_collector **gc_head) {
    char *p = build_prefix(s2, j, n, gc_head);
    char *e = build_env_value(s, flag, gc_head);
    char *suf = build_suffix(s2, j, gc_head);
    return combine_parts(p, e, suf, gc_head);
}

static void process_quote(char *s, int *i, char **cpy, t_gc_collector **gc_head, char quote) {
    (*i)++;
    while (s[*i] && s[*i] != quote) {
        *cpy = join_character(*cpy, s[*i], gc_head);
        (*i)++;
    }
    if (s[*i] == quote)
        (*i)++;
}

static char *process_unquoted(char *s, int *i, t_gc_collector **gc_head) {
    char *cpy = ft_strdup("", gc_head);
    while (s[*i] && s[*i] != '\'' && s[*i] != '\"') {
        cpy = join_character(cpy, s[*i], gc_head);
        (*i)++;
    }
    return cpy;
}

void remove_q_d(t_toknes_list *head, t_gc_collector **gc_head) {
    int i = 0;
    char *cpy = ft_strdup("", gc_head);
    while (head->val[i]) {
        if (head->val[i] == '\'')
            process_quote(head->val, &i, &cpy, gc_head, '\'');
        else if (head->val[i] == '\"')
            process_quote(head->val, &i, &cpy, gc_head, '\"');
        else {
            char *temp = process_unquoted(head->val, &i, gc_head);
            int j = 0;
            while (temp[j])
                cpy = join_character(cpy, temp[j++], gc_head);
        }
    }
    head->val = cpy;
}

static char *build_expand_string(int *i, t_toknes_list *head, int *flag, t_gc_collector **gc_head) {
    char *expand = ft_strdup("", gc_head);
    *flag = 0;
    while (head->val[*i] && !ft_isspace(head->val[*i])) {
        if (not_character_expand(head->val[*i])) {
            *flag = 1;
            break;
        }
        expand = join_character(expand, head->val[*i], gc_head);
        (*i)++;
    }
    return expand;
}

static void handle_dollar_expansion(int *i, char **env, t_toknes_list *head, int flag3, t_gc_collector **gc_head) {
    int flag;
    char *expand = build_expand_string(i, head, &flag, gc_head);
    int j = 0, found = 0;
    while (env && env[j]) {
        if (strcmp_env(env[j], expand, ft_strlen(expand))) {
            found = 1;
            head->val = expand_val(env[j], head->val, *i, flag3, ft_strlen(expand), gc_head);
            if (flag)
                check_is_expandig(head, env, gc_head);
            break;
        }
        j++;
    }
    if (!found) {
        head->val = expand_val("", head->val, *i, flag3, ft_strlen(expand), gc_head);
        if (flag)
            check_is_expandig(head, env, gc_head);
    }
}

static void expand_in_double_quotes(t_toknes_list *head, char **env, t_gc_collector **gc_head, int *i) {
    int dq = 1;
    (*i)++;
    if (head->val[*i] == '\"')
        dq = 0;
    while (dq && head->val[*i]) {
        if (head->val[*i] == '$') {
            while (head->val[*i] && head->val[*i] == '$')
                (*i)++;
            if (not_character_expand(head->val[*i]))
                break;
            handle_dollar_expansion(i, env, head, 0, gc_head);
        } else if (head->val[*i] == '\"')
            dq = 0;
        else
            (*i)++;
    }
}

static void expand_plain(t_toknes_list *head, char **env, t_gc_collector **gc_head, int *i) {
    if (head->val[0] == '$' || head->val[0] == '=')
        head->split_it = 1;
    else
        head->split_it2 = 1;
    while (head->val[*i] && head->val[*i] == '$')
        (*i)++;
    if (!not_character_expand(head->val[*i]))
        handle_dollar_expansion(i, env, head, 1, gc_head);
}

int check_is_expandig(t_toknes_list *head, char **env, t_gc_collector **gc_head) {
    int i = 0, q = 0;
    while (head->val[i]) {
        if (head->val[i] == '\"')
            expand_in_double_quotes(head, env, gc_head, &i);
        else if (head->val[i] == '\'') {
            q = 1;
            skip_q_expand(head->val, &i, &q);
        } else if (head->val[i] == '$')
            expand_plain(head, env, gc_head, &i);
        i++;
    }
    return i;
}

int check_expand(t_toknes_list *head, char **env, t_gc_collector **gc_head) {
    int i = 0, flag = 0, len = ft_strlen(head->val);
    while (head && i < len) {
        if (head->val[i] == '$') {
            i = check_is_expandig(head, env, gc_head);
            flag = 1;
        }
        i++;
    }
    return flag;
}

void expanding(t_toknes_list *token_head, char **env, t_gc_collector **gc_head) {
    t_toknes_list *head = token_head;
    while (head) {
        check_expand(head, env, gc_head);
        remove_q_d(head, gc_head);
        head = head->next;
    }
}
