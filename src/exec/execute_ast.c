/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:52:31 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 00:36:28 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define ERROR "42sh: an error has occured\n"

/*
int			node_while(t_node *node)...
{

}
*/

int			node_and_or_if(t_node *node)
{
	if (node->type == NODE_AND_IF)
	{
		if (execute_ast(node->child[0]) == 0)
			return (execute_ast(node->child[1]));
		return (1);
	}
	if (node->type == NODE_OR_IF)
	{
		if (execute_ast(node->child[0]) != 0)
			return (execute_ast(node->child[1]));
		return (0);
	}
	return (-1);
}

/* ************************************************************************** */

int			execute_ast(t_node *ast)  // bool bg || exec_bg
{
	if (ast == NULL)
		return (-1);
	else if (ast->type == NODE_COMMAND /*&& expand(node, env) == 0*/)
		return (exec_command((t_command *)ast->data));
	else if (ast->type == NODE_AND_IF || ast->type == NODE_OR_IF)
		return (node_and_or_if(ast));
	/*
	else if (((t_token *)ast->data)->type == PIPE)
		return (exec_pipe(ast, env));
	*/
	return (1);
}
