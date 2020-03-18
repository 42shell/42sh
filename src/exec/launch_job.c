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

/*
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
*/

/*
int			node_pipe(t_node *node)
{
	int		fildes[2];

	if (pipe(fildes) == -1)
		return (-1);
	((t_command *)node->child[0]->data)->stdout = fildes[1];
	node_command(node->child[0]);
	close(fildes[1]);
	if (node->child[1]->type == NODE_PIPE)
	{
		((t_command *)node->child[1]->child[0]->data)->stdin = fildes[0];
		node_pipe(node->child[1]);
	}
	else
	{
		((t_command *)node->child[0]->data)->stdin = fildes[0];
		node_command(node->child[1]);
	}
	close(fildes[0]);
	return (0);
}
*/

/*
int			node_command(t_node *node)
{
	t_command	*cmd;

	cmd = (t_command *)node->data;
	if (is_builtin(cmd->argv[0]))
		return (exec_builtin(cmd, g_env->env));
	else if ((cmd->path = get_exec_path(cmd->argv[0])))
		return (exec_binary(cmd, g_env->env));
	return (command_not_found(cmd->argv[0]));
}
*/

/* ************************************************************************** */

int			launch_job(t_job *job)
{
	if (job == NULL)
		return (-1);
	print_ast(job->ast, 0);
	/*
	if (ast->type == NODE_AND_IF || ast->type == NODE_OR_IF)
		return (node_and_or_if(ast));
	*/
	/*
	if (ast->type == NODE_PROCESS && expand(node, env) == 0)
		return (node_command(ast));
	else if (ast->type == NODE_AND_IF || ast->type == NODE_OR_IF)
		return (node_and_or_if(ast));
	*/
	/*
	else if (ast->type == NODE_PIPE)
		return (node_pipe(ast));
	*/
	return (1);
}
