/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


void	print_ast(t_node *ast, int indent)
{
	t_command	*command;
	t_token		*word;
	t_redir		*redir;
	int			i;

	i = 0;
	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
		return (print_ast(ast->left, indent));
	while (i++ < indent)
		printf("    ");
	if (ast->type == NODE_SMPL_CMD)
	{
		command = (t_command *)ast->data;
		word = command->words;
		redir = command->redirs;
		while (word)
		{
			printf("%s ", word->value->str);
			word = word->next;
		}
		while (redir)
		{
			if (redir->left_op)
				printf("%s", redir->left_op->value->str);
			printf("%s", redir->operator->value->str);
			printf("%s ", redir->right_op->value->str);
			redir = redir->next;
		}
		printf("\n");
		return ;
	}
	if (ast->type == NODE_BANG)
	{
		indent++;
		printf("%s\n", "!");
		print_ast(ast->left, indent);
		return ;
	}
	if (ast->type == NODE_PIPE || ast->type == NODE_AND || ast->type == NODE_OR)
	{
		indent++;
		printf("%s\n", ast->type == NODE_PIPE ? "|" : ast->type == AND_IF ? "&&" : "||");
		print_ast(ast->left, indent);
		print_ast(ast->right, indent);
		return ;
	}
	if (ast->type == NODE_SEMI || ast->type == NODE_AMPER || NODE_ROOT)
	{
		indent++;
		printf("%s\n", ast->type == NODE_SEMI ? ";" : ast->type == NODE_AMPER ? "&" : "\\n");
		print_ast(ast->left, indent);
		print_ast(ast->right, indent);
		return ;
	}
}
