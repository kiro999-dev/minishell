/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:48:22 by zkhourba          #+#    #+#             */
/*   Updated: 2025/02/22 18:53:20 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strlen(const char *s)
{
   int i = 0;

   while (s[i])
      i++;
   return (i);
}

char *join_character(char *s,char c)
{
   char *res;
   int   len;
   int   i;

   i = 0;
   len = ft_strlen(s);
   res = malloc(len + 2);
   while (i < len)
   {
      res[i] = s[i];
      i++;
   }
   res[i] = c;
   res[i + 1] = '\0';
   return (res);
}