/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 17:07:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_list_head	*get_sorted_jobs_list(void)
{
	t_list_head	*sorted_list;
	t_job		*job;

	if (!g_jobs)
		return (NULL);
	sorted_list = ft_list_first_head(g_jobs);
	job = g_jobs->next;
	while (job)
	{
		ft_list_add_tail(job, sorted_list);
		job = job->next;
	}
	ft_list_sort(&sorted_list, &ft_cmp);
	sorted_list->prev->next = NULL;
	sorted_list->prev = NULL;
	return (sorted_list);
}

void		del_job_from_list(t_job **head, t_job *job)
{
	remove_job_from_list(head, job);
	process_list_del(&job->processes);
	free(job);
}

void		add_job_to_list(t_job **head, t_job *job, bool set_id)
{
	job->next = *head;
	if (job->next)
		job->next->prev = job;
	if (set_id && !job->id)
		job->id = ++g_jobs_count;
	*head = job;
}

void		remove_job_from_list(t_job **head, t_job *job)
{
	if (job->next)
		job->next->prev = job->prev;
	if (job->prev)
		job->prev->next = job->next;
	else
		*head = job->next;
	job->next = NULL;
	job->prev = NULL;
}

void		add_process_to_job(t_job *job, t_process *process)
{
	if (!job || !process)
		return ;
	process->next = job->processes;
	if (process->next)
		process->next->prev = process;
	job->processes = process;
}
