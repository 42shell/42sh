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
** If we already have a job which has forked processes, pgid of this job is set
** and we want the jobs to have the same pgid
** ex: { ls; ls -a; } | cat
** We want the ls jobs to have the same pgid than cat, if we don t set it
** explicitely, it will be set to the process pid in launch_process
*/

int			eval_compound_list(t_command *command)
{
	t_job	*job;

	while (command)
	{
		job = job_new(command, STDIN_FILENO, STDOUT_FILENO);
		job->pgid = g_shell.jobs->pgid;
		if (command->sep == AMPERSAND)
			job->bg = true;
		launch_job(job);
		command = command->next;
	}
	return (0);
}

/*
** If the group is a subshell, we fork. This function will be called
** again in the fork with g_already_forked = true.
** If the list is more than one command, we set g_already_forked = false
** to be sure that processes will be forked in case we are in a subshell.
** After evaluating the compound_list, jobs containing sub-ast for each command
** in the list have been created, so we set group->list to NULL to avoid double
** free when deleting ast.
*/

int			eval_group_command(t_command *command)
{
	t_process	*process;
	t_group_cmd	*group;

	group = command->value.group;
	if (group->subshell)
	{
		if (!g_already_forked)
		{
			process = process_new(command, STDIN_FILENO, STDOUT_FILENO);
			return (launch_process(process, 0));
		}
	}
	if (set_redir(command->redir_list, true) != 0)
	{
		restore_fds();
		return (g_last_exit_st = 1);
	}
	if (group->list && group->list->next)
		g_already_forked = false;
	eval_compound_list(group->list);
	group->list = NULL;
	restore_fds();
	return (0);
}

int			eval_compound_command(t_command *command)
{
	if (command->type == GROUP)
		return (eval_group_command(command));
	if (command->type == IF_CLAUSE)
		return (eval_if_clause(command));
	return (0);
}