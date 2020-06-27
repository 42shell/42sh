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
/*
void		add_job_to_list(t_job *job)
{
	t_job		*jobdup;
	t_process	*processdup;
	t_process	*process;
	t_process	*prev;
	t_process	*process_;

	//remove_command from g_complete_command list
	if (job->command->next)
		job->command->next->prev = job->command->prev;
	if (job->command->prev)
		job->command->prev->next = job->command->next;
	else if ((g_complete_command = job->command->next))
		g_complete_command->prev = NULL;
	process = job->processes;
	while (process)
	{
		process_ = ft_xmalloc(sizeof(t_process));
		*process_ = *process;
		if (!processdup)
		{
			processdup = process_;
			prev = processdup;
		}
		else
		{
			prev->next = process_;
			prev = prev->next;
		}
		process = process->next;
			
	}
	//dup job and add it to the list
	jobdup = ft_xmalloc(sizeof(t_job));
	//ft_memcpy(jobdup, job, sizeof(t_job));
	*jobdup = *job;
	jobdup->processes = processdup;
	jobdup->next = g_jobs;
	if (jobdup->next)
		jobdup->id = jobdup->next->id + 1;
	g_jobs = jobdup;
}
*/
void		remove_command_from_list(t_command *command)
{
	if (command->next)
		command->next->prev = command->prev;
	if (command->prev)
		command->prev->next = command->next;
	else if ((g_complete_command = command->next))
		g_complete_command->prev = NULL;
}

t_process	*process_list_dup(t_process *process_list)
{
	t_process	*dup_list;
	t_process	*dup;
	t_process	*prev;

	prev = NULL;
	dup_list = NULL;
	while (process_list)
	{
		dup = ft_xmalloc(sizeof(t_process));
		*dup = *process_list;
		if (!dup_list)
		{
			dup_list = dup;
			prev = dup_list;
		}
		else
		{
			prev->next = dup;
			prev = prev->next;
		}
		process_list = process_list->next;
	}
	return (dup_list);
}

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

void		add_job_to_list(t_job **head, t_job *job)// add current job
{
	job->next = *head;
	if (job->next)
		job->id = job->next->id + 1;
	*head = job;
}

void		remove_job_from_list(t_job **head, int job_id)
{
	t_job		*job;
	t_job		*prev;

	prev = NULL;
	job = *head;
	while (job)
	{
		if (job->id == job_id)
		{
			if (prev)
				prev->next = job->next;
			else
				*head = job->next;
		}
		prev = job;
		job = job->next;
	}
}

void		add_process_to_job(t_job *job, t_process *process)
{
	if (!job || !process)
		return ;
	process->next = job->processes;
	job->processes = process;
}

/*
void		remove_job_from_list(int job_id)
{
	t_job		*job;
	t_job		*prev;
	t_list_head	*curr;

	prev = NULL;
	job = g_jobs;
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
				g_jobs = job->next;
		}
		prev = job;
		job = job->next;
	}
}
*/

t_job		*get_job(pid_t pgid)
{
	t_job	*job;

	job = g_jobs;
	while (job)
	{
		if (job->pgid == pgid)
			return (job);
		job = job->next;
	}
	return (NULL);
}
