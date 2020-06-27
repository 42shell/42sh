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

int			eval_pipeline(t_command *command, int in, int out)
{
	t_connection	*pipeline;
	t_process		*process;
	int				fd[2];

	/*
	in = 0;
	while (command == pipe)
	{
		pipe(fd)
		in = fd[0];
		process = process_new(pipeline->left, in, fd[1]);
		return (launch_process(process, fd[0]));
		command = command->right;
	} 
	process = process_new(pipeline->right, fd[0], out);
	launch_process(process, fd[1]);
	*/
	if (pipe(fd) == -1)
		return (-1);
	pipeline = command->value.connection;
	process = process_new(pipeline->right, fd[0], out);
	launch_process(process, fd[1]);
	if (pipeline->left->type == CONNECTION
	&& pipeline->left->value.connection->connector == PIPE)
		return (eval_pipeline(pipeline->left, in, fd[1]));
	process = process_new(pipeline->left, in, fd[1]);
	return (launch_process(process, fd[0]));
}

/*
static int	wait_for_and_or(t_command *command)
{
	if (g_job_control_enabled)
		put_job_fg(g_current_jobs, false);
	else
		wait_for_job(g_current_jobs);
	return (g_last_exit_st);
}
*/

/*
** If we are in a fork, we want the jobs created to have the same pgid than the
** fork.
** ex: { ls && ls } | cat
** We want the ls jobs to have the same pgid than cat, if we don t set it
** explicitely, it will be set to the process pid in launch_process
*/

int			eval_and_or(t_command *command)
{
	t_connection	*and_or;
	t_job			*job;

	g_already_forked = false;
	and_or = command->value.connection;
	if (and_or->left->type == CONNECTION
	&& (and_or->left->value.connection->connector == AND_IF
		|| and_or->left->value.connection->connector == OR_IF))
		eval_command(and_or->left);
	else
	{
		job = job_new(and_or->left, STDIN_FILENO, STDOUT_FILENO);
		job->pgid = g_current_jobs->pgid;
		launch_job(job);
	}
	if ((and_or->connector == AND_IF && g_last_exit_st == 0)
	|| (and_or->connector == OR_IF && g_last_exit_st != 0))
	{
		job = job_new(and_or->right, STDIN_FILENO, STDOUT_FILENO);
		job->pgid = g_current_jobs->pgid;
		launch_job(job);
	}
	return (g_last_exit_st);
}
