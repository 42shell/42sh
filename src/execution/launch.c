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

int		g_pipe[2];

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
		return (1); //code?
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


pid_t		fork_child(int in, int out)
{
	pid_t	pid;

	if ((pid = fork()) == -1)
		return (-1);
	else if (pid == 0)
	{
		if (in != STDIN_FILENO)
		{
			dup2(in, STDIN_FILENO);
			close(in);
		}
		if (out != STDOUT_FILENO)
		{
			dup2(out, STDOUT_FILENO);
			close(out);
		}
	}
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	return (pid);

}

int			launch_process(t_process *process, int in, int out)
{
	if ((process->pid = fork_child(in, out)) == -1)
		return (-1);
	else if (process->pid == 0)
	{
		process->pid = getpid();
		if (g_pipe[0])
			close(g_pipe[0]);
		if (g_pipe[1])
			close(g_pipe[1]);
		eval_ast(process->ast, in, out);
		exit(0);
	}
	add_process(process);
	return (0);
}

int			launch_job(t_job *job)
{
	eval_ast(job->ast, job->stdin, job->stdout);
	wait_for_job(job);
	return (0);
}

/* ************************************************************************** */
/* ************************************************************************** */

int			eval_simple_command(t_node *ast, int in, int out)
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

int			eval_command(t_node *ast, int in, int out)
{
	t_process	*process;

	if (is_builtin(((t_command *)ast->left->data)->words->value->str)
	&& in == STDIN_FILENO && out == STDOUT_FILENO)
		return (eval_ast(ast->left, in, out));
	process = process_new(ast->left);
	launch_process(process, in, out);
	return (0);
}

int			eval_pipeline(t_node *ast, int in, int out) //remove in out
{
	if (pipe(g_pipe) == -1)
		return (-1);
	eval_ast(ast->left, in, g_pipe[1]);
	eval_ast(ast->right, g_pipe[0], out);
	/*
	process = process_new(ast->right);
	launch_process(process, fd[0], out);
	if (ast->left->type == PIPE)
		return (eval_ast(ast->left, in, fd[1]));
	process = process_new(ast->left);
	launch_process(process, in, fd[1]);
	*/
	return (0);
}

int			eval_and_or(t_node *ast, int in, int out)
{
	t_job	*job;

	eval_ast(ast->left, in, out);
	wait_for_job(g_shell.jobs);
	if ((ast->type == AND_IF && g_last_exit_st == 0)
	||(ast->type == OR_IF && g_last_exit_st != 0))
	{
		job = job_new(ast->right, in, out);
		add_job(job);
		launch_job(job);
	}
	return (0);
}

int			eval_ast(t_node *ast, int in, int out)
{
	if (ast->type == AND_IF || ast->type == OR_IF)
		return (eval_and_or(ast, in, out));
	if (ast->type == PIPE)
		return (eval_pipeline(ast, in, out));
	if (ast->type == WORD)
		return (eval_command(ast, in, out));
	if (ast->type == NAME)
		return (eval_simple_command(ast, in, out));
	return (0);
}