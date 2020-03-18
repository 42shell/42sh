/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_job	*get_job(pid_t pgid)
{
	t_list_head	*list;
	t_job		*job;

	list = g_shell.jobs->next;
	while (list != g_shell.jobs)
	{
		job = (t_job *)list->data;
		if (job->pgid == pgid)
			return (job);
		list = list->next;
	}
	return (NULL);
}

bool	job_is_stopped(t_job *job)
{
	t_list_head	*list;
	t_process	*process;

	list = job->processes->next;
	while (list != job->processes)
	{
		process = (t_process *)list->data;
		if (!process->stopped && !process->done)
			return (false);
		list = list->next;
	}
	return (true);
}

bool	job_is_done(t_job *job)
{
	t_list_head	*list;
	t_process	*process;

	list = job->processes->next;
	while (list != job->processes)
	{
		process = (t_process *)list->data;
		if (!process->done)
			return (false);
		list = list->next;
	}
	return (true);
}