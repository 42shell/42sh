/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 15:54:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			eval_simple_command(t_command *command)
{
	t_process		*process;
	t_simple_cmd	*simple;

	simple = command->value.simple;
	if (!simple->is_expand)
	{
		expand(simple);
		if (!(simple->argv = get_argv(simple)))
			return (exec_simple_command(simple));
	}
	if (!g_already_forked
	&& (!is_builtin(simple->argv[0])
		|| (g_job_control_enabled && g_shell.jobs->bg)))
	{
		process = process_new(command, STDIN_FILENO, STDOUT_FILENO);
		return (launch_process(process, 0));
	}
	return (exec_simple_command(simple));
}

/*
** -if we are executing a pipeline, no matter if the command is a builtin
** or not, we don't want to fork cause the pipeline function has already
** call launch_process, so we are in the child.
** -else if the command is not a builtin, or the command is a builtin
** but we are executing an async command, we fork.
** -for async commands, launch_job create a child only if the command is an
** and_or command.
** "ls &" will create 1 child, and execute the command, as for "ls".
** "ls && ls &" will create 1 child, which will fork 2 more childs.
** it allows us to wait for the first 'ls' to finish in the background.
*/

int			eval_pipeline(t_command *command, int in, int out)
{
	t_connection	*pipeline;
	t_process		*process;
	int				fd[2];

	if (pipe(fd) == -1)
		return (-1);
	pipeline = command->value.connection;
	process = process_new(pipeline->right, fd[0], out);
	launch_process(process, fd[1]);
	if (pipeline->left->type == CONNECTION
	&& pipeline->left->value.connection->connector == PIPE)
		return (eval_pipeline(pipeline->left, in, fd[1]));
	process = process_new(pipeline->left, in, fd[1]);
	launch_process(process, fd[0]);
	return (0);
}

int			eval_and_or(t_command *command)
{
	t_connection	*and_or;

	and_or = command->value.connection;
	eval_command(and_or->left);
	if (g_shell.jobs->bg)
		wait_for_job(g_shell.jobs);
	else
		put_job_fg(g_shell.jobs, false);
	if ((and_or->connector == AND_IF && g_last_exit_st == 0)
	|| (and_or->connector == OR_IF && g_last_exit_st != 0))
		eval_command(and_or->right);
	return (0);
}

int			eval_command(t_command *command)
{
	if (command->type == CONNECTION)
	{
		if (command->value.connection->connector == OR_IF
		|| command->value.connection->connector == AND_IF)
			return (eval_and_or(command));
		if (command->value.connection->connector == PIPE)
			return (eval_pipeline(command, STDIN_FILENO, STDOUT_FILENO));
	}
	if (command->type == SIMPLE)
		return (eval_simple_command(command));
	return (0);
}

int			eval_command_list(t_command *command_list)
{
	t_command	*command;
	t_job		*job;

	command = command_list;
	while (command != NULL)
	{
		job = job_new(command, STDIN_FILENO, STDOUT_FILENO);
		add_job(job);
		if (command->flags & CMD_AMPERSAND)
			job->bg = true;
		launch_job(g_shell.jobs);
		command = command->next;
	}
	return (0);
}
