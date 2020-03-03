/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/* ************************************************************************** */

//						EXECUTION FOLDER

/*
** depth first tree traversal
** if we encounter a command or a pipe node, stop traversing and execute it.
** for now, this function actually always stops at the first call.
*/

/*
static int	run(void)
{
	t_ast	*tmp;

	while (ast != NULL)
	{
		if (g_parse_error == NOERR)
		{
			get_all_heredocs(&g_heredocs);
			if (g_parse_error != SILENT_ABORT)
			{
				execute(ast->node, env);
			}
		}
		free_ast_nodes(ast->node, false);
		tmp = ast;
		ast = ast->next;
		free(tmp);
	}
	g_heredocs.nb_children = 0;
	return (0);
}
*/
/* ************************************************************************** */

/*
list             : and_or
                 | and_or separator_op list
*/

t_ast		*list(void)
{
	t_ast	*ast;
	t_node	*root;

	ast = NULL;
	if (g_parser.error || !g_parser.token)
		return (NULL);
	if ((root = and_or(NULL)))
	{
		ast = ast_new();
		ast->root = root;
		if ((ast->sep = separator_op())
		&& g_parser.token && g_parser.token->type != NEWLINE)
			ast->next = list();
		separator();
	}
	get_all_heredocs();
	return (ast);
}

/*

this rule seems to be only useful in batch mode (or maybe in builtins like function definition ??)

I ve added separator() calls to skip empty lines in batch mode.

complete_command : list separator 
                 | list
*/

/*
** use prev instead of last_ast ?
*/

t_ast		*complete_command(void)
{
	t_ast	*ast_list;
	t_ast	*last_ast;

	ast_list = NULL;
	if (g_parser.error)
		return (NULL);
	else if (!g_parser.token)
		g_parser.token = get_next_token();
	separator();
	if ((ast_list = list()))
	{
		if (g_parser.token)
		{
			last_ast = ast_list;
			while (last_ast->next)
				last_ast = last_ast->next;
			last_ast->next = list();
		}
	}
	ft_memdel((void **)&g_parser.heredocs);
	g_parser.heredoc_ptr = NULL;
	if (g_parser.error && g_parser.error != HEREDOC_NO_DELIM)
		ast_del(&ast_list);
	return (ast_list);
}
