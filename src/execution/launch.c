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

static pid_t	fork_child(int in, int out, int to_close)
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

int				launch_process(t_process *process, int to_close)
{
	pid_t	pid;


	if ((pid = fork_child(process->stdin, process->stdout, to_close)) == -1)
		return (-1);
	else if (pid == 0)
	{
		/*
		** if (process->ast->type == '(') //for subshells
		**	g_shell.is_subshell = false;
		*/
		process->pid = getpid();
		if (!g_shell.jobs->pgid)
			g_shell.jobs->pgid = process->pid;
		setpgid(process->pid, g_shell.jobs->pgid);
		//if (!g_shell.jobs->bg)
		//	tcsetpgrp(STDIN_FILENO, g_shell.jobs->pgid);
		g_shell.is_subshell = true;
		g_shell.jobs = NULL;
		/*
		** job = job_new(process->ast, STDIN_FILENO, STDOUT_FILENO);
		** add_job(job);
		** launch_job(job);
		** 
		** ls | (ls && cat)    <- we need a job,
		** ls | (ls && cat &)     but need to check for fg,bg, tcsetpgrp
		*/
		eval_ast(process->ast);
		exit(0);
	}
	else
	{
		process->pid = pid;
		if (!g_shell.jobs->pgid)
			g_shell.jobs->pgid = pid;
		setpgid(process->pid, g_shell.jobs->pgid);
		add_process(process);
	}
	return (pid);
}

int				launch_job(t_job *job)
{
	eval_ast(job->ast);
	if (job->bg)
		put_job_bg(job, false);
	else
		put_job_fg(job, false);
	return (0);
}