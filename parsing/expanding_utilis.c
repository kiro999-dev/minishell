/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utilis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:00:29 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/18 21:38:15 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int not_character_expand(char c) {
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'))
        return 1;
    return 0;
}

void skip_q_expand(char *s, int *ptr_i, int *q_ptr) {
    int i = *ptr_i, q = *q_ptr;
    
    i++;
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

