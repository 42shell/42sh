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

t_job		*get_job_by_str(char *str)
{
	t_job	*job;
	int		id;

	if (str[0] == '%')
	{
		if (str[1] == '+')
			return (g_jobs);
		if (str[1] == '-')
		{
			if (g_jobs && g_jobs->next)
				return (g_jobs->next);
			return (NULL);
		}
		id = ft_atoi(++str);
	}
	else
		id = ft_atoi(str);
	job = g_jobs;
	while (job && job->id != id)
		job = job->next;
	return (job);
}

bool	is_last_job(t_job *job)
{
	if (job && g_jobs == job)
		return (true);
	return (false);
}

bool	is_before_last_job(t_job *job)
{
	if (job && g_jobs && g_jobs->next == job)
		return (true);
	return (false);
}

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
