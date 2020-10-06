/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notif_jobs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:48:20 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	del_done_jobs(void)
{
	t_job	*job;
	t_job	*next;

	job = g_jobs;
	while (job)
	{
		next = job->next;
		if (job_is_done(job) && g_shell.interactive_mode)
			del_job_from_list(&g_jobs, job);
		job = next;
	}
	update_jobs_greatest_id();
}

static void	notif_job(t_job *job)
{
	if (g_shell.interactive_mode)
		print_job(job, false);
	job->notified = true;
}

static void	notif_jobs(void)
{
	t_job	*job;

	if (!g_jobs)
		return ;
	job = g_jobs;
	while (job)
	{
		if (job_is_done(job) && g_shell.interactive_mode)
		{
			if (job->bg || job->processes->signaled)
				print_job(job, false);
		}
		else if (job_is_stopped(job) && !job->notified)
			notif_job(job);
		job = job->next;
	}
}

void		update_jobs(void)
{
	if (!g_jobs)
		return ;
	notif_jobs();
	del_done_jobs();
	update_status();
	notif_jobs();
	del_done_jobs();
}
