/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 19:32:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool			subshell_is_needed(t_command *command)
{
	return ((command->type == GROUP
		&& command->value.group->list->next)
	|| (command->type == CONNECTION
		&& (command->value.connection->connector == AND_IF
			|| command->value.connection->connector == OR_IF)));
}

int				launch_job(t_job *job)
{
	t_process	*process;

	add_job(job);
	if (job->bg)
	{
		g_last_exit_st = 0;
		if (subshell_is_needed(job->command))
		{
			process = process_new(job->command, STDIN_FILENO, STDOUT_FILENO);
			launch_process(process, 0);
		}
		else
			eval_command(job->command);
		put_job_bg(job, false);
		if (g_job_control_enabled)
			ft_printf("[%d] %d\n", job->id + 1, job->pgid);
	}
	else
	{
		eval_command(job->command);
		if (g_job_control_enabled)
			put_job_fg(job, false);
		else
			wait_for_job(job);
	}
	return (0);
}
