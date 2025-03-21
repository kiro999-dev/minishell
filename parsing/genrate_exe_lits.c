/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrate_exe_lits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:44:48 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/21 20:44:35 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	isfile(t_TOKENS type)
{
	if (type == IS_FILE_IN || type == IS_FILE_OUT || type == IS_FILE_APPEND)
		return (1);
	return (0);
}

void	here_doc_case(t_exc_lits **exc_head,
		t_toknes_list **toknz_head_addres, t_file **f_head)
{
	t_toknes_list	*tokenz_head;
	t_exc_lits		*node;

	tokenz_head = *toknz_head_addres;
	tokenz_head = tokenz_head->next;
	node = NULL;
	if (tokenz_head)
	{
		node = creat_node_exc(NULL, LIMTER,
				*f_head, ft_strdup(tokenz_head->val));
		add_back_list(exc_head, node);
		tokenz_head = tokenz_head->next;
	}
	*toknz_head_addres = tokenz_head;
}

char	**cmd_case(t_toknes_list **toknz_head_addres)
{
	char			**cmd;
	t_toknes_list	*tokenz_head;

	tokenz_head = *toknz_head_addres;
	cmd = pars_cmd(tokenz_head);
	tokenz_head = tokenz_head->next;
	*toknz_head_addres = tokenz_head;
	return (cmd);
}

void	generate_list(t_toknes_list *tokenz_head, t_exc_lits **exc_head)
{
	t_file	*f_head;
	char	**cmd;

	while (tokenz_head)
	{
		cmd = NULL;
		f_head = NULL;
		if (tokenz_head->type == CMD)
			cmd = cmd_case(&tokenz_head);
		else if (tokenz_head->type == HER_DOC)
			here_doc_case(exc_head, &tokenz_head, &f_head);
		while (tokenz_head && (isfile(tokenz_head->type)
				|| tokenz_head->type == REDIR_IN
				|| tokenz_head->type == REDIR_OUT
				|| tokenz_head->type == APPEND))
		{
			if (isfile(tokenz_head->type))
				add_list_file(&f_head, tokenz_head->val, tokenz_head->type);
			tokenz_head = tokenz_head->next;
		}
		add_list_exc(exc_head, cmd, CMD, f_head);
		if (tokenz_head && (tokenz_head->type == PIPE
				|| tokenz_head->type == WORD))
			tokenz_head = tokenz_head->next;
	}
}
