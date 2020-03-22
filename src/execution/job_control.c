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
	t_job	*ptr;

	if (!g_shell.jobs)
		g_shell.jobs = job;
	else
	{
		ptr = g_shell.jobs;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = job;
	}
}

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

bool	pipeline_is_done(t_pipeline *pipeline)
{
	t_process	*process;

	process = pipeline->processes;
	while (process)
	{
		if (!process->done)
			return (false);
		process = process->next;
	}
	return (true);
}

bool	job_is_done(t_job *job)
{
	t_pipeline	*pipeline;
	t_process	*process;

	pipeline = job->pipelines;
	while (pipeline)
	{
		process = pipeline->processes;
		while (process)
		{
			if (!process->done)
				return (false);
			process = process->next;
		}
		pipeline = pipeline->next;
	}
	return (true);
}

/* ************************************************************************** */

int		set_process_status(pid_t pid, int status)
{
	t_job		*job;
	t_process	*process;

	job = g_shell.jobs;
	if (!job || !job->curr_pipeline)
		return (0);
	while (job)
	{
		process = job->curr_pipeline->processes;
		while (process)
		{
			if (process->pid == pid)
			{
				process->status = status;
				if (WIFSTOPPED(status))
					process->stopped = 1;
				else
				{
					process->done = true;
					if (WIFSIGNALED(status))
						ft_dprintf(2, "%d: Terminated by signal %d.\n",
						(int)pid, WTERMSIG(process->status));
				}
				return (0);
			}
			process = process->next;
		}
	}
	ft_dprintf(2, "42sh: process %d not found.\n", pid);
	return (-1);
}

void		launch_next_pipeline(t_job *job, int status)
{
	if (job->curr_pipeline->sep == AND_IF && status != 0)
		job->curr_pipeline = job->curr_pipeline->next;
	else if (job->curr_pipeline->sep == OR_IF && status == 0)
		job->curr_pipeline = job->curr_pipeline->next;
	if (job->curr_pipeline && (job->curr_pipeline = job->curr_pipeline->next))
		launch_pipeline(job->curr_pipeline, &job->pgid, job->bg);
}

void		wait_for_job(t_job *job)
{
	pid_t	pid;
	int		status;

	pid = 0;
	status = 0;
	while ((pid = waitpid(WAIT_ANY, &status, WUNTRACED)) > 0 //errors
	&& set_process_status(pid, status) == 0)
	{
		if (job_is_stopped(job))
			return ;
		else if (job_is_done(job))
		{
			g_shell.jobs = g_shell.jobs->next; //del
			return ;
		}
		else if (job->curr_pipeline && job->curr_pipeline->next
		&& pipeline_is_done(job->curr_pipeline))
			launch_next_pipeline(job, status);
	}
}

/* ************************************************************************** */

void		put_job_fg(t_job *job, bool cont)
{
	//tcsetpgrp(STDIN_FILENO, job->pgid);
	if (cont)
	{
		tcsetattr (STDIN_FILENO, TCSADRAIN, &job->tmodes);
		kill(SIGCONT, -job->pgid);
	}
	wait_for_job(job);
	//tcsetpgrp (STDIN_FILENO, g_shell.pgid);
	tcsetattr (STDIN_FILENO, TCSADRAIN, &g_shell.tmodes);
}
