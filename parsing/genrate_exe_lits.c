/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrate_exe_lits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:44:48 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/23 21:39:10 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	isfile(t_TOKENS type)
{
	if (type == IS_FILE_IN || type == IS_FILE_OUT || type == IS_FILE_APPEND)
		return (1);
	return (0);
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
	t_list_here_doc *here_doc_head;
	char	**cmd;
	t_exc_lits *node;
	
	while (tokenz_head)
	{
		cmd = NULL;
		f_head = NULL;
		node = gc_malloc(sizeof(t_exc_lits),1);
		here_doc_head = NULL;
		while(tokenz_head && (tokenz_head->type != PIPE))
		{
			if (tokenz_head && tokenz_head->type == CMD)
			{
				cmd = cmd_case(&tokenz_head);
				node->cmd = cmd;
			}
			if (tokenz_head && isfile(tokenz_head->type))
				add_list_file(&f_head, tokenz_head->val, tokenz_head->type);
			if( tokenz_head && tokenz_head->type == LIMTER)
				here_doc_add(&here_doc_head,ft_strdup(tokenz_head->val),HER_DOC,1);
			if(tokenz_head && tokenz_head->type == PIPE)
				break;
			else if (tokenz_head)
				tokenz_head = tokenz_head->next;
		}
		node->head_files = f_head;
		node->head_here_doc = here_doc_head;
		add_back_list(exc_head,node);
		if(tokenz_head &&  tokenz_head->type == PIPE)
			tokenz_head = tokenz_head->next;
	}
}
