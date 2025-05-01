/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genrate_exe_lits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:44:48 by zkhourba          #+#    #+#             */
/*   Updated: 2025/04/20 17:19:48 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	isfile(t_TOKENS type, t_toknes_list *tokenz_head)
{
	if ((tokenz_head->val[0] != '\0' && tokenz_head->ambiguous)
		&& counting_words(tokenz_head->val, " \t") == 1)
	{
		tokenz_head->ambiguous = 0;
	}
	if (type == IS_FILE_IN || type == IS_FILE_OUT || type == IS_FILE_APPEND)
		return (1);
	return (0);
}

char	**cmd_case(t_toknes_list **toknz_head_addres)
{
	char			**cmd;
	t_toknes_list	*tokenz_head;

	cmd = NULL;
	tokenz_head = *toknz_head_addres;
	cmd = pars_cmd(tokenz_head);
	tokenz_head = tokenz_head->next;
	*toknz_head_addres = tokenz_head;
	return (cmd);
}

void	check_p(t_toknes_list *head1, t_exc_lits *head2 )
{
	int	p;

	p = 0;
	while (head1)
	{
		if (head1->type == PIPE)
			break ;
		else if (head1->type == HER_DOC)
			p = 2;
		else if (head1->type == IS_FILE_IN)
			p = 1;
		head1 = head1->next;
	}
	head2->priority = p;
}

t_exc_lits	*processing_tokenz(t_toknes_list **tokenz_head,
		t_list_here_doc **here_doc_head, t_file	**f_head)
{
	char		**cmd;
	t_exc_lits	*node;

	node = node_inite();
	check_p(*tokenz_head, node);
	while (*tokenz_head && ((*tokenz_head)->type != PIPE))
	{
		cmd = NULL;
		if (tokenz_head && (*tokenz_head)->type == CMD)
		{
			cmd = cmd_case(tokenz_head);
			node->cmd = cmd;
		}
		if (*tokenz_head && isfile((*tokenz_head)->type,*tokenz_head))
			add_list_file(f_head, (*tokenz_head)->val,
				(*tokenz_head)->type, (*tokenz_head)->ambiguous);
		if (*tokenz_head && (*tokenz_head)->type == LIMTER)
			here_doc_add(here_doc_head,
				ft_strdup((*tokenz_head)->val), HER_DOC, 1);
		if (*tokenz_head && (*tokenz_head)->type == PIPE)
			break ;
		else if (*tokenz_head)
			*tokenz_head = (*tokenz_head)->next;
	}
	return (node);
}

void	generate_list(t_toknes_list *tokenz_head, t_exc_lits **exc_head)
{
	t_file			*f_head;
	t_list_here_doc	*here_doc_head;
	t_exc_lits		*node;

	while (tokenz_head)
	{
		f_head = NULL;
		here_doc_head = NULL;
		node = processing_tokenz(&tokenz_head, &here_doc_head, &f_head);
		node->head_files = f_head;
		node->head_here_doc = here_doc_head;
		node->heredoc_fd = -1;
		add_back_list(exc_head, node);
		if (tokenz_head && tokenz_head->type == PIPE)
			tokenz_head = tokenz_head->next;
	}
}
