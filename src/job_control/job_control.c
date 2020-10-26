/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:48:20 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** a stack containing the jobs currently being executed
*/

t_job	*g_current_jobs;

/*
** a stack containing jobs stopped or put in the background
*/

t_job	*g_jobs;

int		g_greatest_job_id;

void	continue_job(t_job *job, bool bg)
{
	t_process *process;

	process = job->processes;
	while (process)
	{
		process->stopped = false;
		process = process->next;
	}
	job->notified = false;
	if (bg)
		put_job_bg(job, true);
	else
		put_job_fg(job, true);
}

void	put_job_bg(t_job *job, bool cont)
{
	extern int	g_last_bg_job_pid;
	extern int	g_last_bg_job_pgid;

	job->bg = true;
	g_last_bg_job_pgid = job->pgid;
	if (job->processes)
		g_last_bg_job_pid = job->processes->pid;
	if (cont)
		kill(-job->pgid, SIGCONT);
}

void	put_job_fg(t_job *job, bool cont)
{
	job->bg = false;
	if (!job->pgid || !job->processes)
		return ;
	if (tcsetpgrp(STDIN_FILENO, job->pgid) == 0)
	{
		if (cont)
		{
			kill(-job->pgid, SIGCONT);
			if (job->has_tmodes)
				tcsetattr(STDIN_FILENO, TCSADRAIN, &job->tmodes);
		}
		wait_for_job(job);
		tcsetpgrp(STDIN_FILENO, g_shell.pgid);
		tcgetattr(STDIN_FILENO, &job->tmodes);
		job->has_tmodes = true;
	}
	else
	{
		ft_dprintf(2, "42sh: process group %d:"
			"couldn't put pgrp in foreground\n", job->pgid);
		kill(-job->pgid, SIGHUP);
		kill(-job->pgid, SIGCONT);
		update_status();
	}
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_shell.tmodes);
}
