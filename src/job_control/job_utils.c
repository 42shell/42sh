/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 17:07:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		update_curr_ptr(t_job *job)
{
	if (g_shell.curr_job == job)
		return ;
	g_shell.prev_job = g_shell.curr_job;
	g_shell.curr_job = job;
}

void		add_job(t_job *job)
{
	job->next = g_shell.jobs;
	if (job->next)
		job->id = job->next->id + 1;
	g_shell.jobs = job;
}

void		add_process(t_process *process)
{
	if (!g_shell.jobs)
		return ;
	process->next = g_shell.jobs->processes;
	g_shell.jobs->processes = process;
}

void		remove_job_from_list(int job_id)
{
	t_job	*job;
	t_job	*prev;

	prev = NULL;
	job = g_shell.jobs;
	while (job)
	{
		if (job->id == job_id)
		{
			if (g_shell.prev_job && g_shell.prev_job == job)
				g_shell.prev_job = NULL;
			if (g_shell.curr_job && g_shell.curr_job == job)
			{
				g_shell.curr_job = g_shell.prev_job;
				g_shell.prev_job = NULL;
			}
			if (prev)
				prev->next = job->next;
			else
				g_shell.jobs = job->next;
		}
		prev = job;
		job = job->next;
	}
}

t_job		*get_job(pid_t pgid)
{
	t_job	*job;

	job = g_shell.jobs;
	while (job)
	{
		if (job->pgid == pgid)
			return (job);
		job = job->next;
	}
	return (NULL);
}
