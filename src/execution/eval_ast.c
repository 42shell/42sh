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

	if (ast->left->type == NODE_SMPL_CMD)
	{
		if (g_shell.is_subshell
		|| is_builtin(((t_command *)ast->left->data)->words->value->str))
			return (eval_simple_command(ast->left));
		process = process_new(ast->left, STDIN_FILENO, STDOUT_FILENO);
		launch_process(process, 0);
	}
	return (0);
}

int			eval_pipeline(t_node *ast, int in, int out)
{
	t_process	*process;
	int			fd[2];

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
	t_job	*job;

	eval_ast(ast->left);
	put_job_fg(g_shell.jobs, false);
	if (ast->left->type == BANG)
		g_last_exit_st = g_last_exit_st ? 0 : 1;
	if ((ast->type == NODE_AND && g_last_exit_st == 0)
	|| (ast->type == NODE_OR && g_last_exit_st != 0))
	{
		job = job_new(ast->right, STDIN_FILENO, STDOUT_FILENO);
		add_job(job); //because i m not deleting processes when they re done
		launch_job(job);
	}
	return (0);
}

int			eval_separator_op(t_node *ast)
{
	t_process	*process;

	if (ast->type == NODE_AMPER)
	{
		//process = process_new(ast->left, STDIN_FILENO, STDOUT_FILENO);
		//add_process(process);
		g_shell.jobs->bg = true;//?
		//return (launch_process(process, 0));
	}
	return (eval_ast(ast->left));
}

int			eval_ast(t_node *ast)
{
	t_job	*job;

	if (!ast)
		return (0);
	if (ast->type == NODE_SEMI || ast->type == NODE_AMPER)
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