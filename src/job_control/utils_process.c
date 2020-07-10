/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 17:07:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_process	*get_process_in(t_job *list, pid_t pid)
{
	t_job		*job;
	t_process	*process;

	job = list;
	while (job)
	{
		process = job->processes;
		while (process)
		{
			if (process->pid == pid)
				return (process);
			process = process->next;
		}
		job = job->next;
	}
	return (NULL);
}

t_process			*get_process(pid_t pid)
{
	t_process	*process;

	if (!(process = get_process_in(g_current_jobs, pid))
	&& !(process = get_process_in(g_jobs, pid)))
		return (NULL);
	return (process);
}
