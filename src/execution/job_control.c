/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	notif_jobs(void)
{
	t_job	*job;
	t_job	*next;

	update_status();
	job = g_shell.jobs;
	while (job)
	{
		next = job->next;
		if (job_is_done(job))
		{
			if (job->bg)
				print_job(job, JOB_DONE);
			remove_job_from_list(job->pgid);
			process_del(&job->processes);
			ast_del(&job->ast);
			free(job);
		}
		else if (job_is_stopped(job) && !job->notified)
		{
			print_job(job, JOB_STOPPED);
			job->notified = true;
		}
		job = next;
	}
}

void	wait_for_job(t_job *job)
{
	pid_t		pid;
	int			status;

	pid = 0;
	status = 0;
	if (!job->processes)
		return ;
	while (!job_is_done(job))
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (pid > 0 && mark_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
		else if (pid < 0)
		{
			ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
			break ;
		}
	}
	g_last_exit_st = WEXITSTATUS(job->processes->status);
}

void	continue_job(t_job *job, bool bg)
{
	mark_job_as_running(job);
	if (bg)
		put_job_bg(job, true);
	else
    	put_job_fg(job, true);
}

void	put_job_bg(t_job *job, bool cont)
{
	job->bg = true;
	if (cont)
		kill(-job->pgid, SIGCONT);
}

void	put_job_fg(t_job *job, bool cont)
{
	job->bg = false;
	tcsetpgrp(STDIN_FILENO, job->pgid);
	if (cont)
	{
		kill(-job->pgid, SIGCONT);
		tcsetattr (STDIN_FILENO, TCSADRAIN, &job->tmodes);
	}
	wait_for_job(job);
	tcsetpgrp(STDIN_FILENO, g_shell.pgid);
	tcgetattr(STDIN_FILENO, &job->tmodes);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_shell.tmodes);
}