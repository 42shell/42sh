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
	t_job		*job;
	t_job		*prev;
	t_list_head	*curr;

	prev = NULL;
	job = g_shell.jobs;
	while (job)
	{
		if (job->id == job_id)
		{
			if ((curr = get_job_current_list_elem(job)))
			{
				remove_current_list_elem(curr);
				free(curr);
			}
			if (g_curr_job && g_curr_job->next == g_curr_job)
				ft_memdel((void **)&g_curr_job);
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
