/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:28:32 by zkhourba          #+#    #+#             */
/*   Updated: 2025/02/25 18:03:01 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

typedef enum e_TOKENS
{
    PIPE
    ,REDIR_OUT,REDIR_IN,STRING,DOLLAR,CMD,ARG,APPEND
    ,HER_DOC
}t_TOKENS;

typedef struct s_toknes
{
    t_TOKENS        type;
    char*           val;
    struct s_toknes *next;
}t_toknes_list;

char   	**ft_split(const char *s, char c);
char    *join_character(char *s,char c);
char	*ft_strdup(const char *s1);
int     ft_strlen(const char *s);
#endif
