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

void	add_command_to_list(t_command **head, t_command *command)
{
	command->next = *head;
	if (*head)
		(*head)->prev = command;
	*head = command;
}

void	remove_command_from_list(t_command **head, t_command *command)
{
	if (command->next)
		command->next->prev = command->prev;
	if (command->prev)
		command->prev->next = command->next;
	else if ((*head = command->next))
		(*head)->prev = NULL;
}

void	add_job_to_list(t_job **head, t_job *job, bool set_id)
{
	job->next = *head;
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
	t_job		*ptr;
	t_job		*prev;

	prev = NULL;
	ptr = *head;
	while (ptr)
	{
		if (ptr == job)
		{
			if (prev)
				prev->next = ptr->next;
			else
				*head = ptr->next;
		}
		prev = ptr;
		ptr = ptr->next;
	}
}

void	add_process_to_job(t_job *job, t_process *process)
{
	if (!job || !process)
		return ;
	process->next = job->processes;
	job->processes = process;
}
