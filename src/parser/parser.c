/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

/*
** g_heredocs' children are pointers to the heredoc nodes currently in the AST
*/

void		del_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	free_ast_nodes((*ast)->node, false);
	del_ast(&((*ast)->next));
	ft_memdel((void **)ast);
}

t_ast		*get_ast(void)
{
	t_ast	*ast;
	t_node	*root;

	ast = NULL;
	if (!(g_token = get_next_token()))
		return (NULL);
	if ((root = and_or()))
	{
		ast = ft_xmalloc(sizeof(t_ast));
		ast->node = root;
		if (g_token && (g_token->type == SEMI || g_token->type == AMPERSAND))
		{
			//separator rule
			ast->run_in_background = (g_token->type == AMPERSAND);
			token_del(&g_token);
			ast->next = get_ast();
		}
	}
	return (ast);
}

/*
** gets the list of ASTs for the input, then executes all of them sequentially.
** ex: the input "ls ; cat; wc" gets a list of 3 asts
** asts are trees containing t_tokens in their "data" fields.
** one node can have an infinite number of children.
*/

/*
int				parse(void)
{
	t_ast	*ast;
	t_ast	*ptr;

	g_parse_error = NOERR;
	if ((ast = get_ast()) && !g_parse_error)
	{
		//run();
		ptr = ast;
		while (ptr)
		{
			print_ast(ptr->node, 0);//run(ast, env);
			printf("\n");
			ptr = ptr->next;
		}
	}
	del_ast(&ast);
	return (0);
}
*/
