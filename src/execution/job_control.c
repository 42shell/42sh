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

t_process	*process_new(t_node *ast)
{
	t_process	*process;

	process = (t_process *)ft_xmalloc(sizeof(t_process));
	process->ast = ast;
	return (process);
}

t_job		*job_new(t_node *ast, int stdin, int stdout)
{
	t_job	*job;

	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->stdin = 0;
	job->stdout = 1;
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

int		set_last_status(pid_t pid, int status)
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
	ft_dprintf(2, "42sh: process %d not found.\n", pid);
	return (-1);
}

void		wait_for_job(t_job *job)
{
	pid_t	pid;
	int		status;

	pid = 0;
	status = 0;
	/*
	t_process *process;

	process = job->processes;
	while (process)
	{
		//printf("%d: !done\n", process->pid);
		waitpid(process->status, &process->status, 0);
		process = process->next;
	}
	g_last_exit_st = status;
	*/
	while (!job_is_done(job))
	{
		pid = waitpid(WAIT_ANY, &status, 0);
		g_last_exit_st = status;
		if (pid > 0 && 
		set_last_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
		if (pid == 0)
		{
			ft_dprintf(2, "42sh: no process ready to report.\n", pid);
			break ;
		}
		if (pid < 0)
		{
			ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
			break ;
		}
	}

	/*
	while ((pid = waitpid(WAIT_ANY, &status, WUNTRACED)) > 0 //errors
	&& set_last_status(pid, status) == 0)
	{
		if (job_is_done(job))
		{
			g_shell.jobs = g_shell.jobs->next; //del
			g_last_exit_st = status;
			return ;
		}
	}
	*/
}