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

void	free_ast_nodes(t_node *node, bool par_is_pattern)
{
	int		i;
	//bool	cur_is_pattern;

	(void)par_is_pattern;
	//cur_is_pattern = false;
	if (node == NULL)
		return ;
	else if (node->type == NODE_COMMAND)
		command_del((t_command **)&node->data);
	else if (node->data != NULL /*&& !par_is_pattern*/)
	{
		token_del((t_token **)&node->data);
		//cur_is_pattern = (node_token(node)->type == PATTERN);
	}
	//else if (par_is_pattern)
	//	array_destroy(node->data);
	i = 0;
	while (i < node->nb_children)
		free_ast_nodes(node->child[i++], /*cur_is_pattern*/ 0);
	free(node->child);
	free(node);
}

int		parse_error(int code, char *near)
{
	if (g_parser.error)
		return (0);
	else if (code == HEREDOC_NO_DELIM)
		ft_dprintf(2,
		"42sh: warning: here-document delimited by end-of-file (wanted '%s')\n",
		near);
	else if (code != SILENT_ABORT && near)
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
			printf("%s ", command->argv[i++]->value->str);
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
