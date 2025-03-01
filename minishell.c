/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhourba <zkhourba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:37:53 by zkhourba          #+#    #+#             */
/*   Updated: 2025/03/01 09:17:32 by zkhourba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_list(t_toknes_list *head)
{
    t_toknes_list *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->val);
        free(tmp);
    }
}

t_toknes_list *creat_node(char *val,t_TOKENS type)
{
	t_toknes_list *node;
	node = malloc(sizeof(t_toknes_list));
	if(!node)
		return (printf("malloc fail\n"),NULL);
	node->val = val;
	node->type = type;
	node->next = NULL;
	return (node);
}

void	add(t_toknes_list **head,char *val,t_TOKENS type)
{
		t_toknes_list *node;
		t_toknes_list *tmp;
		
		if(head == NULL)
			return ;
		node = creat_node(val,type);
		if (!node)
			return ;
		if(*head == NULL)
			*head = node;
		else
		{
			tmp = *head;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = node;
			node->prv = tmp;
		}
		
}
t_ast *creat_node_ast(char **cmd,char *value,t_TOKENS type)
{
	t_ast *node;
	node = malloc(sizeof(t_ast));
	if(!node)
		return (printf("malloc fail\n"),NULL);
	node->cmd = cmd;
	node->type = type;
	node->val = value;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

char **pars_cmd(t_toknes_list **token) 
{
	
    t_toknes_list *current = *token;
    int count = 0;
    char **cmd;
    int i = 0;
    // Count the number of WORD/CMD tokens
    while (current && (current->type != PIPE)) {
		
		if(current->type == WORD || current->type == CMD)
        	count++;
        current = current->next;
    }

    cmd = malloc(sizeof(char *) * (count + 1)); // +1 for NULL terminator
    if (!cmd) return NULL;
    current = *token;
	while (current && (current->type == WORD || current->type == CMD)) {
	    cmd[i] = ft_strdup(current->val);
        current = current->next;
	    i++;
    }
    cmd[i] = NULL; // Null-terminate the array
	
    // Advance the original token pointer past processed tokens
    *token = current;
    return cmd;
}
t_ast *generate_tree(t_toknes_list **token_ptr)
{
    t_ast *root = NULL;
    t_ast *current = NULL;
    t_toknes_list *token = *token_ptr;

    while (token) {
        if (token->type == PIPE) 
        {
            t_ast *pipe_node = creat_node_ast(NULL, "|", PIPE);
            if (!root) {
                // Handle syntax error: pipe with no left command
                fprintf(stderr, "Syntax error near pipe\n");
                return NULL;
            }
            printf("pipe is the root\n");
            pipe_node->left = root;
            root = pipe_node;
            token = token->next;
            // Parse right side of the pipe
            t_ast *right = generate_tree(&token);
            if (!right) {
                // Syntax error: pipe with no right command
                fprintf(stderr, "Syntax error after pipe\n");
                return NULL;
            }
            root->right = right;
        } 
        else if (token->type == CMD || token->type == WORD) 
        {
            char **cmd = pars_cmd(&token);
            if (!cmd) return NULL;
            t_ast *cmd_node = creat_node_ast(cmd, "cmd", CMD);
            if (!root) 
            {
                root = cmd_node;
                current = root;
            } else 
            {
                current->right = cmd_node;
                current = cmd_node;
            }
        } 
        else if (token->type == REDIR_IN || token->type == REDIR_OUT ||  token->type == APPEND) 
        {
			t_ast *redir_nod = creat_node_ast(NULL,token->val,token->type);
			if (!root)
			{
				root = redir_nod;
				current = root;
			}
			else {
                current->right = redir_nod;
                current = redir_nod;
            }
			// Verify there's a filename following
			if (!token->next ||( token->next->type != WORD && token->next->type != IS_FILE && token->next->type != IS_FILE_APPEND)) 
            {
				// print(token->next->val,token->next->type)
				fprintf(stderr, "Syntax error: missing filename after '%s'\n", token->val);
			    return NULL;
			}
			if (token->type == REDIR_IN) 
            {
				current->input_file = ft_strdup(token->next->val);
			} 
			else if (token->type == REDIR_OUT) 
            {
				current->output_file = ft_strdup(token->next->val);
				current->append_mode = 0;
			} 
			else if (token->type == APPEND) 
            {
				current->output_file = ft_strdup(token->next->val);
            	current->append_mode = 1;
			}
			// Move past both the redirection token and filename
			token = token->next->next;
		}
    }
    *token_ptr = token; // Update the token pointer
    return root;
}
#include <stdio.h>
#include <stdbool.h>

void print_ast_tree(t_ast *node, int depth, bool is_last, bool is_left) {
    static char prefix[1024] = {0};
    char new_prefix[1024];
    
    if (depth > 0) {
        // Add vertical lines for parent connections
        for (int i = 0; i < depth-1; i++) {
            if (prefix[i] == '`' || prefix[i] == ' ') {
                printf("    ");
            } else {
                printf("│   ");
            }
        }

        // Add current node connection
        if (is_left) {
            printf("left :%s├── ", is_last ? "┌──" : "│  ");
        } else {
            printf("right :%s└── ", is_last ? "└──" : "│  ");
        }
    }

    // Print current node
    if (node->type == PIPE) {
        printf("-│-\n");
    } else if (node->type == CMD) {
        // Print command with arguments
        printf("CMD [");
        for (int i = 0; node->cmd[i]; i++) {
            printf("%s%s", node->cmd[i], node->cmd[i+1] ? " " : "");
        }
        printf("]\n");
    } else if (node->type == REDIR_IN) {
        printf("%s infile:%s\n", node->val,node->input_file);
    }
	else if (node->type == REDIR_OUT) {
        printf("%s file:%s\n", node->val,node->output_file);
    }
	else if (node->type == APPEND) {
        printf("%s file:%s\n", node->val,node->output_file);
    }

    // Build prefix for children
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", 
            prefix, 
            (is_last || depth == 0) ? "    " : "│   ");

    // Recursively print children
    if (node->left) {
        print_ast_tree(node->left, depth + 1, !node->right, true);
    }
    if (node->right) {
        print_ast_tree(node->right, depth + 1, true, false);
    }
}

// Wrapper function
void print_ast(t_ast *root) {
    printf("\n");
    print_ast_tree(root, 0, true, false);
    printf("\n");
}

void free_ast(t_ast *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    if (node->cmd) {
        for (int i = 0; node->cmd[i]; i++) free(node->cmd[i]);
        free(node->cmd);
    }
    free(node->input_file);
    free(node->output_file);
    free(node);
}


int main() {
    char *buff = readline("> ");
    t_toknes_list *head = NULL;
    t_ast *ast = NULL;

    lex(buff, &head);
    ast = generate_tree(&head);
    
    // Execute the AST here (not implemented)
	if(ast)
    	print_ast(ast);
    free(buff);
    free_list(head);
    // free_ast(ast);
    return 0;
}