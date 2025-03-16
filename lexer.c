/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:48:22 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/15 23:19:41 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isspace(int c) {
    if ((c >= 9 && c <= 13) || c == 32)
        return 1;
    return 0;
}

void add_token(t_tok *data_tok, t_toknes_list **head, t_gc_collector **gc_head);

static char *read_single_quote_token(char *s, int *i_ptr, t_tok *data, t_gc_collector **gc_head) {
    int i = *i_ptr, q = 0;
    char *token = ft_strdup("", gc_head);
    if (s[i] == '\'') {
        q = !q;
        token = join_character(token, s[i], gc_head);
        i++;
    }
    while (s[i]) {
        if (s[i] == '\'')
            q = !q;
        if (ft_isspace(s[i]) && !q)
            break;
        token = join_character(token, s[i], gc_head);
        if (s[i] == '\'' && !data->is_redir_in && !data->is_redir_out &&
            !data->is_append && !data->is_here_d) {
            i++;
            break;
        }
        i++;
    }
    *i_ptr = i;
    return token;
}

void single_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head, t_gc_collector **gc_head) {
    char *token = read_single_quote_token(s, i_ptr, data, gc_head);
    if (token[0] == '\0')
        return;
    data->token = token;
    add_token(data, head, gc_head);
}

static char *read_double_quote_token(char *s, int *i_ptr, t_tok *data, t_gc_collector **gc_head) {
    int i = *i_ptr, q = 0;
    char *token = ft_strdup("", gc_head);
    if (s[i] == '\"') {
        q = !q;
        token = join_character(token, s[i], gc_head);
        i++;
    }
    while (s[i]) {
        if (s[i] == '\"')
            q = !q;
        if (ft_isspace(s[i]) && !q)
            break;
        token = join_character(token, s[i], gc_head);
        if (s[i] == '\"' && !data->is_redir_in && !data->is_redir_out &&
            !data->is_append && !data->is_here_d) {
            i++;
            break;
        }
        i++;
    }
    *i_ptr = i;
    return token;
}

void double_q(int *i_ptr, char *s, t_tok *data, t_toknes_list **head, t_gc_collector **gc_head) {
    char *token = read_double_quote_token(s, i_ptr, data, gc_head);
    if (token[0] == '\0')
        return;
    data->token = token;
    add_token(data, head, gc_head);
}

int pipe_symbol(int *i_ptr, int *is_cmd, t_toknes_list **head, t_gc_collector **gc_head) {
    int i = *i_ptr;
    add(head, ft_strdup("|", gc_head), PIPE, 0, gc_head);
    *is_cmd = 1;
    *i_ptr = i;
    return 1;
}

int redir_in(int *i_ptr, char *s, t_toknes_list **head, t_tok *d, t_gc_collector **gc_head) {
    int i = *i_ptr;
    i++;
    if (s[i] == '<') {
        add(head, ft_strdup("<<", gc_head), HER_DOC, 0, gc_head);
        d->is_here_d = 1;
    } else {
        add(head, ft_strdup("<", gc_head), REDIR_IN, 0, gc_head);
        d->is_redir_in = 1;
        i--;
    }
    *i_ptr = i;
    return 1;
}

int redir_out(int *i_ptr, char *s, t_toknes_list **head, t_tok *d, t_gc_collector **gc_head) {
    int i = *i_ptr;
    i++;
    if (s[i] == '>') {
        add(head, ft_strdup(">>", gc_head), APPEND, 0, gc_head);
        d->is_append = 1;
    } else {
        add(head, ft_strdup(">", gc_head), REDIR_OUT, 0, gc_head);
        d->is_redir_out = 1;
        i--;
    }
    *i_ptr = i;
    return 1;
}

void print(char *s, t_TOKENS type) {
    if (s)
        printf("%s--
