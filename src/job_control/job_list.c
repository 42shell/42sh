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

void	del_job_from_list(t_job **head, t_job *job)
{
	remove_job_from_list(head, job);
	process_list_del(&job->processes);
	free(job);
}

void	add_command_to_list(t_command **head, t_command *command)
{
	command->next = *head;
	if (*head)
		(*head)->prev = command;
	*head = command;
}

void	add_job_to_list(t_job **head, t_job *job, bool set_id)
{
	job->next = *head;
	if (job->next)
		job->next->prev = job;
	if (set_id && !job->id)
	{
		if (!job->next)
			job->id = 1;
		else
			job->id = job->next->id + 1;
	}
	*head = job;
}

void	remove_job_from_list(t_job **head, t_job *job)
{
	if (job->next)
		job->next->prev = job->prev;
	if (job->prev)
		job->prev->next = job->next;
	else
		*head = job->next;
}

void	add_process_to_job(t_job *job, t_process *process)
{
	if (!job || !process)
		return ;
	process->next = job->processes;
	if (process->next)
		process->next->prev = process;
	job->processes = process;
}
