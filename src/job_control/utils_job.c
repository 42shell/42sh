/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_job.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 17:07:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_job		*job_dup(t_job *job)
{
	t_job		*dup_job;
	t_process	*dup_process;

	dup_job = ft_xmalloc(sizeof(t_job));
	*dup_job = *job;
	dup_process = process_list_dup(job->processes);
	dup_job->processes = dup_process;
	return (dup_job);
}

/*
** we compare the pointers cause different jobs can have same pgid when
** dealing with compound_lists.
*/

bool	job_is_in_list(t_job *list, t_job *job_to_check)
{
	t_job	*job;

	job = list;
	while (job)
	{
		if (job == job_to_check)
			return (true);
		job = job->next;
	}
	return (false);
}

bool	job_is_stopped(t_job *job)
{
	t_process	*process;

	if (job_is_done(job))
		return (false);
	process = job->processes;
	while (process)
	{
		if (!process->done && !process->stopped)
			return (false);
		process = process->next;
	}
	return (true);
}

bool	job_is_done(t_job *job)
{
	t_process	*process;

	process = job->processes;

	while (process)
	{
		if (!process->done)
			return (false);
		process = process->next;
	}
	return (true);
}
