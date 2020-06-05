/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 15:54:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			eval_simple_command(t_command *command) //exec_command()
{
	t_process		*process;
	t_simple_cmd	*simple;

	simple = command->value.simple;
	if (!simple->is_expand)
	{
		expand(simple);
		if (!(simple->argv = get_argv(simple)))
			return (exec_simple_command(command));
	}
	if (!g_already_forked && !is_builtin(simple->argv[0]))
	{
		process = process_new(command, STDIN_FILENO, STDOUT_FILENO);
		return (launch_process(process, 0));
	}
	return (exec_simple_command(command));
}

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

	g_already_forked = false;
	and_or = command->value.connection;
	eval_command(and_or->left);
	if (!g_job_control_enabled)
		wait_for_job(g_shell.jobs);
	else
		put_job_fg(g_shell.jobs, false);
	if ((and_or->connector == AND_IF && g_last_exit_st == 0)
	|| (and_or->connector == OR_IF && g_last_exit_st != 0))
		eval_command(and_or->right);
	return (0);
}

int			eval_group_command(t_command *command)
{
	t_process	*process;
	t_group_cmd	*group;
	t_command	*cmd;

	group = command->value.group;
	if (group->subshell)
	{
		if (!g_already_forked)
		{
			process = process_new(command, STDIN_FILENO, STDOUT_FILENO);
			return (launch_process(process, 0));
		}
		if (group->list->next)
			g_already_forked = false;
	}
	if (set_redir(command->value.group->redir_list, true) != 0)
	{
		restore_fds();
		return (g_last_exit_st = 1);
	}
	cmd = group->list;
	while (cmd)
	{
		eval_command(cmd);
		if (!g_job_control_enabled)
			wait_for_job(g_shell.jobs);
		else
			put_job_fg(g_shell.jobs, false);
		cmd = cmd->next;
	}
	restore_fds();
	return (0);
}

int			eval_command(t_command *command)
{
	if (command->type == GROUP)
		return (eval_group_command(command));
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

int			eval_complete_command(t_command *complete_command)
{
	t_command	*command;
	t_job		*job;

	command = complete_command;
	while (command != NULL)
	{
		job = job_new(command, STDIN_FILENO, STDOUT_FILENO);
		if (command->sep == AMPERSAND)
			job->bg = true;
		launch_job(job);
		command = command->next;
	}
	return (0);
}
