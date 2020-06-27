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

void	remove_command_from_list(t_command *command)
{
	if (command->next)
		command->next->prev = command->prev;
	if (command->prev)
		command->prev->next = command->next;
	else if ((g_complete_command = command->next))
		g_complete_command->prev = NULL;
}

void	add_job_to_list(t_job **head, t_job *job)
{
	job->next = *head;
	if (job->next)
		job->id = job->next->id + 1;
	*head = job;
}

void	remove_job_from_list(t_job **head, int job_id)
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

void	add_process_to_job(t_job *job, t_process *process)
{
	if (!job || !process)
		return ;
	process->next = job->processes;
	job->processes = process;
}
