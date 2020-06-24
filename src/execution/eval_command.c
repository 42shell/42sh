/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/28 19:44:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** If the command is not expanded, we expand it.
** If the expansion result in no argv (only redirs), we can execute it.
** If the command is not a builtin and we are not in a fork yet, we fork.
** This function will be called again in the fork and skip expansion and
** forking if statements.
*/

int			eval_simple_command(t_command *command)
{
	t_process		*process;
	t_simple_cmd	*simple;

	simple = command->value.simple;
	if (!simple->is_expand)
	{
		if (expand(simple) == 1)
			return (g_last_exit_st = 1);
		if (!(simple->argv = get_argv(simple)))
			return (exec_simple_command(simple));
	}
	if (!g_already_forked && !is_builtin(simple->argv[0]))
	{
		process = process_new(command, STDIN_FILENO, STDOUT_FILENO);
		return (launch_process(process, 0));
	}
	return (exec_simple_command(simple));
}

int			eval_command(t_command *command)
{
	if (command->flags & CMD_COMPOUND)
		return (eval_compound_command(command));
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
