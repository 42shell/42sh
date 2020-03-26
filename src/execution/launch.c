/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			exec_builtin(char **argv, char **env)
{
	(void)env;
	//if (ft_strequ(argv[0], "env"))
	//	builtin_env(argv, env);
	if (ft_strequ(argv[0], "exit"))
		builtin_exit(argv);
	else if (ft_strequ(argv[0], "unsetenv"))
		builtin_unsetenv(argv);
	else if (ft_strequ(argv[0], "setenv"))
		builtin_setenv(argv);
	else if (ft_strequ(argv[0], "echo"))
		builtin_echo(argv);
	else if (ft_strequ(argv[0], "cd"))
		builtin_cd(argv);
	//restore_fds(); this should be called even in forks
	return (0); //errors ?
}

int			exec_binary(char **argv, char **env)
{
	char	*path;

	if (!(path = get_exec_path(argv[0])))
	{
		ft_dprintf(2, "42sh: %s: command not found\n", argv[0]);
		exit (1); //code?
	}
	else if (execve(path, argv, env) == -1)
	{
		ft_dprintf(2, "42sh: %s: cannot execute command\n", argv[0]);
		free(path);
		return (1); //code ?
	}
	free(path);
	return (0);
}

/* ************************************************************************** */

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
	process = process_new(ast->right, fd[0], out);
	launch_process(process, fd[1]);
	if (ast->left->type == NODE_PIPE)
		return (eval_pipeline(ast->left, in, fd[1]));
	process = process_new(ast->left, in, fd[1]);
	launch_process(process, fd[0]);
	/* or
	if (ast->left->type == NODE_PIPELINE)
		eval_pipeline(ast->left, in, fd[1]);
	else
	{
		process = process_new(ast->left, in, fd[1]);
		launch_process(process, fd[0]);
	}
	process = process_new(ast->right, fd[0], out);
	launch_process(process, fd[1]);
	*/
	return (0);
}

int			eval_and_or(t_node *ast)
{
	t_job	*job;

	//addnewjob here?
	eval_ast(ast->left);
	wait_for_job(g_shell.jobs);
	if (ast->left->type == BANG)
		g_last_exit_st = g_last_exit_st ? 0 : 1;
	if ((ast->type == NODE_AND && g_last_exit_st == 0)
	|| (ast->type == NODE_OR && g_last_exit_st != 0))
	{
		job = job_new(ast->right, STDIN_FILENO, STDOUT_FILENO);
		add_job(job);
		launch_job(job);
	}
	return (0);
}

int			eval_ast(t_node *ast)
{
	//addnewjob here?
	if (!ast)
		return (0);
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
