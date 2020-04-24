/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/24 01:07:19 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			g_exec_status;

int			eval_simple_command(t_command *command, int in, int out,
		int fd_to_close)
{
	t_process		*process;
	t_simple_cmd	*simple;
	bool			builtin;

	simple = command->value.simple;
	expand(simple, g_env);
	simple->argv = get_argv(simple);
	if (!simple->argv)
	{
		set_redir(simple, true);
		restore_fds();
		return (0);
	}
	builtin = is_builtin(simple->argv[0]);
	if ((g_exec_status & EXEC_PIPELINE) || !builtin)
	{
		process = process_new(command, in, out);
		return (launch_process(process, fd_to_close, false));
	}
	if (set_redir(simple, true) != 0)
		return (1);
	if (is_builtin(simple->argv[0]))
		return (exec_builtin(simple->argv));
	return (exec_binary(simple->argv, g_env->env));
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

int			eval_pipeline(t_connection *pipeline, int in, int out)
{
	int			fd[2];

	g_exec_status |= EXEC_PIPELINE;
	if (pipe(fd) == -1)
		return (-1);
	if (pipeline->left->type == CONNECTION)
		eval_pipeline(pipeline->left->value.connection, in, fd[1]);
	else
		eval_simple_command(pipeline->left, in, fd[1], fd[0]);
	eval_simple_command(pipeline->right, fd[0], out, fd[1]);
	return (0);
}

int			eval_and_or(t_connection *and_or)
{
	g_exec_status |= EXEC_AND_OR;
	eval_command(and_or->left);
	if (g_shell.jobs->bg)
		wait_for_job(g_shell.jobs, true);
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
			return (eval_and_or(command->value.connection));
		if (command->value.connection->connector == PIPE)
			return (eval_pipeline(command->value.connection,
						STDIN_FILENO, STDOUT_FILENO));
	}
	if (command->type == SIMPLE)
		return (eval_simple_command(command, STDIN_FILENO, STDOUT_FILENO, 0));
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
		{
			g_exec_status |= EXEC_ASYNC;
			job->bg = true;
		}
		launch_job(g_shell.jobs);
		command = command->next;
	}
	return (0);
}
