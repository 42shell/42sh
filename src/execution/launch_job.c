/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_job.c                                       :+:      :+:    :+:   */
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
static void		reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	//signal(SIGCHLD, SIG_DFL);
}

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
		process->pid = getpid();
		if (!g_shell.jobs->pgid)
			g_shell.jobs->pgid = process->pid;
		setpgid(process->pid, g_shell.jobs->pgid);
		if (!g_shell.jobs->bg)
			tcsetpgrp(STDIN_FILENO, g_shell.jobs->pgid);
		reset_signals();
		//printf("fork\n");
		//g_shell.is_subshell = true;
		//eval_ast(process->ast);
		//exit(0);
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
	t_process	*process;

	if (job->bg)
	{
		if (job->ast->type == NODE_AND || job->ast->type == NODE_OR)
		{
			process = process_new(job->ast, STDIN_FILENO, STDOUT_FILENO);
			if (launch_process(process, 0) == 0)
			{
				eval_ast(process->ast);
				exit(0);
			}
		}
		else
			eval_ast(job->ast);
		put_job_bg(job, false);
	}
	else
	{
		eval_ast(job->ast);
		put_job_fg(job, false);
	}
	return (0);
}