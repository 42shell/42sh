/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			g_exec_status;

int			eval_simple_command(t_node *ast)
{
	t_command	*command;
	char		**argv;

	command = (t_command *)ast->data;
	//fix and set redirs
	argv = get_argv(command);
	if (is_builtin(argv[0]))
		return (exec_builtin(argv, g_env->env));
	return (exec_binary(argv, g_env->env));
	//restore fds
}

int			eval_command(t_node *ast)
{
	t_process	*process;
	bool		builtin;

	if (ast->left->type == NODE_SMPL_CMD)
	{
		builtin = is_builtin(((t_command *)ast->left->data)->words->value->str);
		if (!(g_exec_status & EXEC_ST_PIPELINE)
		&& (!builtin || (g_exec_status & EXEC_ST_ASYNC)))
		{
			process = process_new(ast->left, STDIN_FILENO, STDOUT_FILENO);
			return (launch_process(process, 0));
		}
		return (eval_simple_command(ast->left));
	}
	return (0);
}

int			eval_pipeline(t_node *ast, int in, int out)
{
	t_process	*process;
	int			fd[2];

	g_exec_status |= EXEC_ST_PIPELINE;
	if (pipe(fd) == -1)
		return (-1);
	if (ast->left->type == NODE_PIPE)
		eval_pipeline(ast->left, in, fd[1]);
	else
	{
		process = process_new(ast->left, in, fd[1]);
		launch_process(process, fd[0]);
	}
	process = process_new(ast->right, fd[0], out);
	launch_process(process, fd[1]);
	return (0);
}

int			eval_and_or(t_node *ast)
{
	g_exec_status |= EXEC_ST_AND_OR;
	eval_ast(ast->left);
	if (g_shell.jobs->bg)
		wait_for_job(g_shell.jobs);
	else
		put_job_fg(g_shell.jobs, false);
	if (ast->left->type == BANG)
		g_last_exit_st = g_last_exit_st ? 0 : 1;
	if ((ast->type == NODE_AND && g_last_exit_st == 0)
	|| (ast->type == NODE_OR && g_last_exit_st != 0))
		eval_ast(ast->right);
	return (0);
}

int			eval_separator_op(t_node *ast)
{
	t_job		*job;

	g_exec_status = 0;
	job = job_new(ast->left, STDIN_FILENO, STDOUT_FILENO);
	add_job(job);
	if (ast->type == NODE_AMPER)
	{
		g_exec_status |= EXEC_ST_ASYNC;
		job->bg = true;
	}
	launch_job(job);
	eval_ast(ast->right);
	return (0);
}

int			eval_ast(t_node *ast)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_SEMI || ast->type == NODE_AMPER || ast->type == NODE_ROOT)
		return (eval_separator_op(ast));
	if (ast->type == NODE_AND || ast->type == NODE_OR)
		return (eval_and_or(ast));
	if (ast->type == BANG)
		ast = ast->left;
	if (ast->type == NODE_PIPE)
		return (eval_pipeline(ast, STDIN_FILENO, STDOUT_FILENO));
	if (ast->type == NODE_CMD)
		return (eval_command(ast));
	if (ast->type == NODE_SMPL_CMD)
		return (eval_simple_command(ast));
	return (0);
}