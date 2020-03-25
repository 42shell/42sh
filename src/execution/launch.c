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

/*
static void	set_child_pgid(t_process *process, pid_t *pgid, bool bg)
{
	process->pid = getpid();
	*pgid = *pgid ? *pgid : process->pid;
	setpgid(process->pid, *pgid);
	//if (!bg)
	//	tcsetpgrp(STDIN_FILENO, *pgid);
}
*/

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

pid_t		fork_child(int in, int out, int to_close)
{
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (-1);
	else if (pid == 0)
	{
		if (to_close)
			close(to_close);
		if (in != STDIN_FILENO)
			dup2(in, STDIN_FILENO);
		if (out != STDOUT_FILENO)
			dup2(out, STDOUT_FILENO);
	}
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	return (pid);
}

int			launch_process(t_process *process, int to_close)
{
	pid_t	pid;

	if ((pid = fork_child(process->stdin, process->stdout, to_close)) == -1)
		return (-1);
	else if (pid == 0)
	{
		process->pid = getpid();
		if (process->stdin != STDIN_FILENO)
			g_shell.stdin = process->stdin;
		if (process->stdout != STDOUT_FILENO)
			g_shell.stdout = process->stdout;
		eval_ast(process->ast);
		exit(1);
	}
	else
	{
		process->pid = pid;
		add_process(process);
	}
	return (pid);
}

int			launch_job(t_job *job)
{
	eval_ast(job->ast);
	wait_for_job(job);
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

	if (ast->left->type == NODE_SIMPLE_COMMAND
	&& (is_builtin(((t_command *)ast->left->data)->words->value->str)
	|| (g_shell.stdin != STDIN_FILENO || g_shell.stdout != STDOUT_FILENO)))
		return (eval_simple_command(ast->left));
	process = process_new(ast->left, STDIN_FILENO, STDOUT_FILENO);
	launch_process(process, 0);
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
	if (ast->left->type == NODE_PIPELINE)
		return (eval_pipeline(ast->left, in, fd[1]));
	process = process_new(ast->left, in, fd[1]);
	launch_process(process, fd[0]);
	return (0);
}

int			eval_and_or(t_node *ast)
{
	t_job	*job;

	eval_ast(ast->left);
	wait_for_job(g_shell.jobs);
	if (ast->left->type == BANG)
		g_last_exit_st = g_last_exit_st ? 0 : 1;
	if ((ast->type == NODE_AND_IF && g_last_exit_st == 0)
	|| (ast->type == NODE_OR_IF && g_last_exit_st != 0))
	{
		job = job_new(ast->right, STDIN_FILENO, STDOUT_FILENO);
		add_job(job);
		launch_job(job);
	}
	return (0);
}

int			eval_ast(t_node *ast)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_AND_IF || ast->type == NODE_OR_IF)
		return (eval_and_or(ast));
	if (ast->type == BANG)
		ast = ast->left;
	if (ast->type == NODE_PIPELINE)
		return (eval_pipeline(ast, STDIN_FILENO, STDOUT_FILENO));
	if (ast->type == NODE_COMMAND)
		return (eval_command(ast));
	if (ast->type == NODE_SIMPLE_COMMAND)
		return (eval_simple_command(ast));
	return (0);
}
