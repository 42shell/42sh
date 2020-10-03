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

extern t_command	*g_complete_command;

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
		{
			simple->is_expand = false;
			return (exec_simple_command(simple));
		}
	}
	simple->is_expand = false;
	if (!g_already_forked && simple->argv && !is_builtin(simple->argv[0]))
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
	if (command->type == AND_OR)
		return (eval_and_or(command));
	if (command->type == PIPELINE)
		return (eval_pipeline(command));
	if (command->type == SIMPLE)
		return (eval_simple_command(command));
	return (0);
}

int			eval_complete_command(t_command *complete_command)
{
	t_command	*command;
	t_job		*job;

	command = complete_command;
	while (!g_interrupt && command != NULL)
	{
		job = job_new(command, STDIN_FILENO, STDOUT_FILENO);
		if (command->sep == AMPERSAND)
			job->bg = true;
		launch_job(job);
		add_binary_msgs_to_hash();
		command = command->next;
	}
	return (g_last_exit_st);
}
