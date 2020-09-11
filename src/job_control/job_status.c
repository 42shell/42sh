/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 17:07:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool	job_is_running(t_job *job)
{
	t_process	*process;

	process = job->processes;
	while (process)
	{
		if (!process->done && !process->stopped)
			return (true);
		process = process->next;
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
		if (!process->stopped && !process->done)
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
