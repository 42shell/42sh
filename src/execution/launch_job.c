/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/24 01:05:43 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				launch_job(t_job *job)
{
	t_process	*process;

	if (job->bg)
	{
		if (job->command->type == CONNECTION
		&& (job->command->value.connection->connector == AND_IF
			||  job->command->value.connection->connector == OR_IF))
		{
			process = process_new(job->command, STDIN_FILENO, STDOUT_FILENO);
			launch_process(process, 0);
		}
		else
			eval_command(job->command);
		put_job_bg(job, false);
		ft_printf("[%d] %d\n", job->id + 1, job->pgid);
	}
	else
	{
		eval_command(job->command);
		put_job_fg(job, false);
	}
	return (0);
}
