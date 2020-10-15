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

bool	job_is_in_list(t_job *list, t_job *job)
{
	while (list)
	{
		if (job == list)
			return (true);
		list = list->next;
	}
	return (false);
}

int		cmp_job_id(void *a, void *b)
{
	if (((t_job *)a)->id < ((t_job *)b)->id)
		return (-1);
	if (((t_job *)a)->id > ((t_job *)b)->id)
		return (1);
	return (0);
}

void	update_jobs_greatest_id(void)
{
	t_job	*job;
	int		max;

	max = 0;
	if (!(job = g_jobs))
	{
		g_greatest_job_id = 0;
		return ;
	}
	while (job)
	{
		if (job->id > max)
			max = job->id;
		job = job->next;
	}
	g_greatest_job_id = max;
}
