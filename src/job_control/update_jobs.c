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

void	update_jobs(bool notif)
{
	t_job			*job;
	t_job			*next;

	update_status();
	job = g_jobs;
	while (job)
	{
		next = job->next;
		if (g_shell.interactive_mode && job_is_done(job))
		{
			if (notif)
				print_job(job, false);
			del_job_from_list(&g_jobs, job);
		}
		else if (job_is_stopped(job) && !job->notified)
		{
			remove_job_from_list(&g_jobs, job);
			add_job_to_list(&g_jobs, job, false);
			if (notif && g_shell.interactive_mode)
				print_job(job, false);
			job->notified = true;
		}
		job = next;
	}
}
