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

t_job		*g_done_jobs = NULL;

static void	enqueue_job(t_job **head, t_job *job)
{
	t_job	*ptr;

	if (!job)
		return ;
	if (!*head)
	{
		*head = job;
		return ;
	}
	ptr = *head;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = job;
	job->prev = ptr;
}

void		del_done_jobs(void)
{
	t_job	*job;
	t_job	*next;

	job = g_jobs;
	while (job)
	{
		next = job->next;
		if (job_is_done(job))
		{
			if (job->notified)
				del_job_from_list(&g_jobs, job);
			else
			{
				remove_job_from_list(&g_jobs, job);
				enqueue_job(&g_done_jobs, job);
			}
		}
		job = next;
	}
	if (g_done_jobs)
		enqueue_job(&g_jobs, g_done_jobs);
	update_jobs_greatest_id();
	g_done_jobs = NULL;
}

static void	notif_jobs(void)
{
	t_job	*job;

	if (!(job = g_jobs) || !g_shell.interactive_mode)
		return ;
	while (job->next)
		job = job->next;
	while (job)
	{
		if (!job->notified)
		{
			if (SHOW_NOTIF
			&& (job_is_stopped(job)
			|| (job_is_done(job) && (job->bg || job->processes->signaled))))
			{
				print_job(job, false);
				job->notified = true;
			}
		}
		job = job->prev;
	}
}

/*
** We first notif the jobs whose status have been updated while waiting for a
** fg job in wait_for_job(). Then we call update_status and notif again
*/

void		update_jobs(void)
{
	if (!g_jobs)
		return ;
	notif_jobs();
	update_status();
	notif_jobs();
	del_done_jobs();
}
