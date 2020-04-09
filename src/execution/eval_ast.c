/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 23:29:30 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			g_exec_status;

int			eval_simple_command(t_command *command)
{
	t_process		*process;
	t_simple_cmd	*simple;
	char			**argv;
	bool			builtin;

	simple = command->value.simple;
	argv = (char **)simple->argv->array;
	builtin = is_builtin(simple->argv->array[0]);
	if (!(g_exec_status & EXEC_PIPELINE)
			&& (!builtin || (g_exec_status & EXEC_ASYNC)))
	{
		process = process_new(command, STDIN_FILENO, STDOUT_FILENO);
		return (launch_process(process, 0, false));
	}
	set_redir(simple, true);
	if (is_builtin(argv[0]))
		return (exec_builtin(argv, g_env->env));
	return (exec_binary(argv, g_env->env));
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
	t_process	*process;
	int			fd[2];

	g_exec_status |= EXEC_PIPELINE;
	if (pipe(fd) == -1)
		return (-1);
	if (pipeline->left->type == CONNECTION)
		eval_command(pipeline->left, in, fd[1]);
	else
	{
		process = process_new(pipeline->left, in, fd[1]);
		launch_process(process, fd[0], false);
	}
	process = process_new(pipeline->right, fd[0], out);
	launch_process(process, fd[1], false);
	return (0);
}

int			eval_and_or(t_connection *and_or, int in, int out)
{
	g_exec_status |= EXEC_AND_OR;
	eval_command(and_or->left, in, out);
	if (g_shell.jobs->bg)
		wait_for_job(g_shell.jobs);
	else
		put_job_fg(g_shell.jobs, false);
	if ((and_or->connector == AND_IF && g_last_exit_st == 0)
			|| (and_or->connector == OR_IF && g_last_exit_st != 0))
		eval_command(and_or->right, in, out);
	return (0);
}

int			eval_command(t_command *command, int in, int out)
{
	if (command->type == CONNECTION)
	{
		if (command->value.connection->connector == OR_IF
				|| command->value.connection->connector == AND_IF)
			return (eval_and_or(command->value.connection, in, out));
		if (command->value.connection->connector == PIPE)
			return (eval_pipeline(command->value.connection, in, out));
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
		{
			g_exec_status |= EXEC_ASYNC;
			job->bg = true;
		}
		launch_job(g_shell.jobs);
		command = command->next;
	}
	return (0);
}
