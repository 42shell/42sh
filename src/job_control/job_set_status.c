/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_set_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 16:47:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	mark_job_as_running(t_job *job)
{
	t_process *process;

	process = job->processes;
	while (process)
	{
		process->stopped = false;
		process = process->next;
	}
	job->notified = false;
}

/*
** We only set last_exit_st if stdout == 1 to ignore the exit status of the
** first commands of a pipeline
** TODO: only print necessary signals
*/

void	set_process_status(t_process *process, int status)
{
	process->status = status;
	if (WIFSTOPPED(status))
	{
		process->stopped = true;
		process->signaled = WSTOPSIG(process->status);
	}
	else
	{
		process->done = true;
		if (WIFEXITED(status) && process->stdout == 1)
			g_last_exit_st = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			process->signaled = WTERMSIG(process->status);
			if (process->stdout == 1)
				g_last_exit_st = process->signaled + 128;
			if (process->signaled != 13 && process->signaled != 2)
				ft_dprintf(2, "%d: Terminated by signal %d.\n",
				(int)process->pid, WTERMSIG(process->status));
		}
	}
}

int		mark_status(pid_t pid, int status)
{
	t_job		*job;
	t_process	*process;

	job = g_shell.jobs;
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

void	update_status(void)
{
	pid_t	pid;
	int		status;

	pid = 0;
	while ((pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED)) > 0)
	{
		if (mark_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
	}
	if (pid < 0 && errno != ECHILD)
		ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
}
