/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_compound.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/28 19:44:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** If we are in a fork, we want the jobs created to have the same pgid
** than the previous fork.
** ex: { ls; ls -a; } | cat
** We want the ls jobs to have the same pgid than cat, if we don t set it
** explicitely, it will be set to the process pid in launch_process
*/

int			eval_compound_list(t_command *command)
{
	t_job	*job;

	while (!g_interrupt && command)
	{
		job = job_new(command, STDIN_FILENO, STDOUT_FILENO);
		if (g_current_jobs)
			job->pgid = g_current_jobs->pgid;
		if (command->sep == AMPERSAND)
			job->bg = true;
		launch_job(job);
		command = command->next;
	}
	return (g_last_exit_st);
}

/*
** If the command is a subshell and there is only another subshell in
** the compound list, we have useless parens like in "((ls))".
** We dont fork in this case.
*/

static int	subshell_is_useless(t_command *command)
{
	t_command	*compound_list;

	compound_list = command->value.compound_list;
	if ((compound_list->type == GROUP
	&& (compound_list->flags & CMD_SUBSHELL)
	&& !compound_list->next))
		return (true);
	return (false);
}

/*
** If the group is a subshell, we fork. This function will be called
** again in the fork with g_already_forked = true.
** If the list is more than one command, we set g_already_forked = false
** to be sure that processes will be forked in case we are already in a fork.
*/

int			eval_group_command(t_command *command)
{
	t_process	*process;
	t_list_head	*fd_backups;

	fd_backups = NULL;
	if ((command->flags & CMD_SUBSHELL) && !g_already_forked
	&& !subshell_is_useless(command))
	{
		process = process_new(command, STDIN_FILENO, STDOUT_FILENO);
		return (launch_process(process, 0));
	}
	if (set_redir(command->redir_list, &fd_backups) != 0)
		return (g_last_exit_st = 1);
	if (command->value.compound_list && command->value.compound_list->next)
		g_already_forked = false;
	eval_compound_list(command->value.compound_list);
	restore_fds(&fd_backups);
	return (g_last_exit_st);
}

int			eval_compound_command(t_command *command)
{
	if (command->type == GROUP)
		return (eval_group_command(command));
	if (command->type == IF_CLAUSE)
		return (eval_if_clause(command));
	if (command->type == WHILE_CLAUSE)
		return (eval_while_clause(command));
	return (0);
}
