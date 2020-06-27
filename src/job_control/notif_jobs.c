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

void	notif_jobs(void)
{
	t_job	*job;
	t_job	*next;

	update_status();
	job = g_jobs;
	while (job)
	{
		next = job->next;
		if (job_is_done(job))
		{
			if (job->bg && g_shell.interactive_mode)
				print_job(job, true);
			remove_job_from_list(&g_jobs, job->id);
			process_list_del(&job->processes);
			command_del(&job->command);
			free(job);
		}
		else if (job_is_stopped(job) && !job->notified)
		{
			if (g_shell.interactive_mode)
				print_job(job, true);
			job->notified = true;
		}
		job = next;
	}
}
