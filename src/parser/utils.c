/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/* ************************************************************************** */
/* ************************************************************************** */

void	print_ast(t_node *ast, size_t indent_level)
{
	size_t		i;
	size_t		n;
	t_command	*command;
	t_redir		*redir;

	n = 0;
	while (n++ < indent_level)
		write(1, "  ", 2);
	if (ast->type == NODE_COMMAND)
	{
		i = 0;
		command = (t_command *)ast->data;
		if (!command->argv && !command->redirs)
			return ;
		while (command->argv && command->argv[i])
			printf("%s ", command->argv[i++]);
		i = 0;
		while (command->redirs && command->redirs[i])
		{
			redir = command->redirs[i++];
			if (redir->left_op)
				printf("%s ", redir->left_op->value->str);
			printf("%s ", redir->operator->value->str);
			printf("%s ", redir->right_op->value->str);
		}
		printf("\n");
		return ;
	}
	else if (ast->data != NULL)
		ft_printf("%s\n", ((t_token *)ast->data)->value->str);
	else
		ft_printf("NULL DATA\n");
	fflush(stdout);
	if (ast->nb_children > 0)
	{
		i = 0;
		while (i < (size_t)ast->nb_children)
			print_ast(ast->child[i++], indent_level + 1);
	}
}
