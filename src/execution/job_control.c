/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	add_job(t_job *job)
{
	job->next = g_shell.jobs;
	g_shell.jobs = job;
}

void	add_process(t_process *process)
{
	process->next = g_shell.jobs->processes;
	g_shell.jobs->processes = process;
}

t_process	*process_new(t_node *ast, int stdin, int stdout)
{
	t_process	*process;

	process = (t_process *)ft_xmalloc(sizeof(t_process));
	process->stdin = stdin;
	process->stdout = stdout;
	process->ast = ast;
	return (process);
}

t_job		*job_new(t_node *ast, int stdin, int stdout)
{
	t_job	*job;

	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->stdin = stdin;
	job->stdout = stdout;
	job->ast = ast;
	return (job);
}

/*
t_job	*get_job(pid_t pgid)
{
	(void)pgid;
	return (NULL);
}

bool	job_is_stopped(t_job *job)
{
	(void)job;
	return (false);
}
*/

bool	job_is_done(t_job *job)
{
	t_process	*process;

	process = job->processes;
	while (process)
	{
		if (!process->done)
			return (false);
		process = process->next;
	}
	return (true);
}

/* ************************************************************************** */

void	set_process_status(t_process *process, int status)
{
	process->status = status;
	if (WIFSTOPPED(status))
		process->stopped = 1;
	else
	{
		process->done = true;
		if (WIFSIGNALED(status))
			ft_dprintf(2, "%d: Terminated by signal %d.\n",
			(int)process->pid, WTERMSIG(process->status));
	}
}

int		update_status(pid_t pid, int status)
{
	t_job		*job;
	t_process	*process;

	job = g_shell.jobs;
	if (!job || !job->processes)
		return (0);
	while (job)
	{
		process = job->processes;
		while (process)
		{
			if (process->pid == pid)
			{
				set_process_status(process, status);
				return (0);
			}
			process = process->next;
		}
		job = job->next;
	}
	return (-1);
}

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

void		wait_for_job(t_job *job)
{
	pid_t	pid;
	int		status;

	pid = 0;
	status = 0;
	t_process *process;
	/*
	process = job->processes;
	while (process)
	{
		//we should be able to do this instead of wait ANY and loop through all the jobs,
		//as waitpid will return the statuses of childs already terminated in the function
		//we call before the prompt to notif bg jobs
		waitpid(process->status, &process->status, 0);
		process = process->next;
	}
	g_last_exit_st = status;
	*/
	while (!job_is_done(job))
	{
		pid = waitpid(WAIT_ANY, &status, 0);
		g_last_exit_st = WEXITSTATUS(status);
		if (pid > 0 && update_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
		else if (pid == 0)
		{
			ft_dprintf(2, "42sh: no process ready to report.\n", pid);
			break ;
		}
		else if (pid < 0)
		{
			ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
			break ;
		}
	}
	process = job->processes;
	if (process)
	{
		while (process->next)
			process = process->next;
		g_last_exit_st = WEXITSTATUS(process->status);
	}
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
		g_shell.is_subshell = true;
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