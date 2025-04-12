/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:48:22 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/12 16:55:02 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_token(char c)
{
	char	*s;

	s = "<>|\'\"";
	if (ft_strchr(s, c) == NULL)
		return (0);
	else
		return (1);
}

int	ft_isspace(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void	init_tok(t_tok *data_tok)
{
	data_tok->is_append = 0;
	data_tok->is_cmd = 1;
	data_tok->is_here_d = 0;
	data_tok->is_redir_in = 0;
	data_tok->is_redir_out = 0;
	data_tok->q = 0;
	data_tok->dq = 0;
	data_tok->join_me = 1;
}

int	isparam_for_file(t_tok *data_tok)
{
	
	if (data_tok->is_redir_out || data_tok->is_here_d
		|| data_tok->is_append || data_tok->is_redir_in)
			return (1);
	else
		return (0);
	
}

void	not_token_case(t_tok *data_tok,
	t_toknes_list **head, int *ptr_i, char *s)
{
	gen_word(data_tok, s, ptr_i);
	add_token(data_tok, head);
}
