/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 15:54:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		eval_pipeline(t_command *command)
{
	t_command	*pipeline;
	t_process	*process;
	int			stdin;
	int			fd[2];

	stdin = STDIN_FILENO;
	pipeline = command->value.pipeline;
	while (!g_interrupt && pipeline->next)
	{
		if (pipe(fd) == -1)
			return (-1);
		process = process_new(pipeline, stdin, fd[1]);
		launch_process(process, fd[0]);
		pipeline = pipeline->next;
		stdin = fd[0];
	}
	process = process_new(pipeline, stdin, STDOUT_FILENO);
	return (launch_process(process, fd[1]));
}

/*
** If we are in a fork, we want the jobs created to have the same pgid
** than the fork.
** ex: { ls && ls } | cat
** We want the ls jobs to have the same pgid than cat, if we don t set it
** explicitely, it will be set to the process pid in launch_process
*/

int		eval_and_or(t_command *command)
{
	t_command	*and_or;
	t_job		*job;

	g_already_forked = false;
	and_or = command->value.and_or;
	while (!g_interrupt && and_or)
	{
		if (!(and_or->flags & (CMD_AND_IF | CMD_OR_IF))
		|| ((and_or->flags & CMD_AND_IF) && g_last_exit_st == 0)
		|| ((and_or->flags & CMD_OR_IF) && g_last_exit_st != 0))
		{
			job = job_new(and_or, STDIN_FILENO, STDOUT_FILENO);
			if (g_current_jobs)
				job->pgid = g_current_jobs->pgid;
			launch_job(job);
		}
		and_or = and_or->next;
	}
	return (g_last_exit_st);
}
