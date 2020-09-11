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

static void	update_greatest_id(void)
{
	t_job	*job;

	if (!(job = g_jobs))
		g_greatest_job_id = 0;
	while (job)
	{
		if (job->id > g_greatest_job_id)
			g_greatest_job_id = job->id;
		job = job->next;
	}
}

static void	notif_job(t_job *job)
{
	if (g_shell.interactive_mode)
		print_job(job, false);
	job->notified = true;
}

void		update_jobs(void)
{
	t_job			*job;
	t_job			*next;

	if (!(job = g_jobs))
		return ;
	update_status();
	while (job)
	{
		next = job->next;
		if (job_is_done(job) && g_shell.interactive_mode)
		{
			if (job->bg || job->processes->signaled)
				print_job(job, false);
			del_job_from_list(&g_jobs, job);
		}
		else if (job_is_stopped(job) && !job->notified)
			notif_job(job);
		job = next;
	}
	update_greatest_id();
}
