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

static void	bubble_up_and_notif(t_job *job, bool print_notif)
{
	remove_job_from_list(&g_jobs, job);
	add_job_to_list(&g_jobs, job, false);
	if (print_notif && g_shell.interactive_mode)
		print_job(job, false);
	job->notified = true;
}

/*
** If job is done:
** 		-if and we are in batch mode, we just ignore it. There is no
**		notification, and we don t delete it from the list cause it needs to be
** 		printed if jobs builtin is called.
** 		-if this function is called from fg/bg/jobs, we don t delete the job
** 		from the list, cause it needs to be notified in the builtin.
** If the job is stopped:
** 		-we put it on top of the stack
*/

void		update_jobs(bool called_from_main, bool print_notif)
{
	t_job			*job;
	t_job			*next;
	struct timespec	time;

	if (!(job = g_jobs))
		return ;
	time.tv_sec = 0;
	time.tv_nsec = 0x10000000;
	nanosleep(&time, NULL);
	while (job)
	{
		next = job->next;
		if (job_is_done(job) && g_shell.interactive_mode)
		{
			if (print_notif && job->bg)
				print_job(job, false);
			if (called_from_main)
				del_job_from_list(&g_jobs, job);
		}
		else if (job_is_stopped(job) && !job->notified)
			bubble_up_and_notif(job, print_notif);
		job = next;
	}
	update_greatest_id();
}
