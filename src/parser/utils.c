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

t_token	*node_token(t_node *node)
{
	return ((t_token *)(node->data));
}

void	free_ast_nodes(t_node *node, bool par_is_pattern)
{
	int		i;
	bool	cur_is_pattern;

	cur_is_pattern = false;
	if (node == NULL)
		return ;
	else if (is_process(node))
		process_del((t_process **)&node->data);
	else if (node->data != NULL && !par_is_pattern)
	{
		cur_is_pattern = (node_token(node)->type == PATTERN);
		ft_dstr_del((void **)&node_token(node)->value);
		free(node->data);
	}
	else if (par_is_pattern)
		array_destroy(node->data);
	i = 0;
	while (i < node->nb_children)
		free_ast_nodes(node->child[i++], cur_is_pattern);
	free(node->child);
	free(node);
}

int		parse_error(int code, char *near)
{
	if (g_parser.error)
		return (0);
	else if (g_parser.error != SILENT_ABORT && near)
		ft_dprintf(2,
		"42sh: syntax error near unexpected token '%s'\n",
		near);
	g_parser.error = code;
	free(near);
	return (0);
}

/* ************************************************************************** */
/* ************************************************************************** */

void	print_ast(t_node *ast, size_t indent_level)
{
	size_t		i;
	size_t		n;
	t_process	*process;
	t_redir		*redir;

	n = 0;
	while (n++ < indent_level)
		write(1, "  ", 2);
	if (is_process(ast))
	{
		i = 0;
		process = (t_process *)ast->data;
		if (!process->argv && !process->redirs)
			return ;
		while (process->argv && process->argv[i])
			printf("%s ", process->argv[i++]->value->str);
		i = 0;
		while (process->redirs && process->redirs[i])
		{
			redir = process->redirs[i++];
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
