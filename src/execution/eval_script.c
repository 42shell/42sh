/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_script.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/05 15:54:13 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** g_loop_lvl
** ctrl-C		:	g_interrupt = true
** 				:	g_interrupt_is_due_to_break = false;
** break n		:	g_interrupt = true
** 				:	g_interrupt_is_due_to_break = true;
** 				:	g_break_n = n
** eval_while	:	g_loop_lvl++;
** 					if g_interrupt && g_interrupt_is_due_to_break
** 						if --g_break_n == 0
** 							g_interrupt_is_due_to_break = false
** 							g_interrupt = false
** 						break
*/

int			eval_while_clause(t_command *command)
{
	t_if_clause	*if_clause;
	t_list_head	*fd_backup;

	fd_backup = NULL;
	g_already_forked = false;
	if_clause = command->value.if_clause;
	if (expand_cmd(command) != 0
	|| set_redir(command->redir_list, &fd_backup) != 0)
	{
		restore_fds(&fd_backup);
		return (g_last_exit_st = 1);
	}
	while (!g_interrupt
	&& eval_compound_list(if_clause->if_part) == 0 && !g_interrupt)
		eval_compound_list(if_clause->then_part);
	restore_fds(&fd_backup);
	return (0);
}

/*
** We set g_already_forked=false cause we need to evaluate the if_part in a
** subshell to avoid exit if we are already in a subshell.
** We keep trace of the old already_forked status to avoid eventual unnecessary
** fork when executing the then_part.
*/

static int	eval_if_statement(t_if_clause *if_clause)
{
	int		old_already_forked;

	old_already_forked = g_already_forked;
	g_already_forked = false;
	eval_compound_list(if_clause->if_part);
	g_already_forked = old_already_forked;
	if (g_last_exit_st == 0)
	{
		eval_compound_list(if_clause->then_part);
		return (1);
	}
	return (0);
}

int			eval_if_clause(t_command *command)
{
	t_if_clause	*if_clause;
	t_list_head	*fd_backup;

	fd_backup = NULL;
	if (expand_cmd(command) != 0
	|| set_redir(command->redir_list, &fd_backup) != 0)
	{
		restore_fds(&fd_backup);
		return (g_last_exit_st = 1);
	}
	while (!g_interrupt && command)
	{
		if_clause = command->value.if_clause;
		if (if_clause->if_part)
		{
			if (eval_if_statement(if_clause) == 1)
				break ;
		}
		else
			eval_compound_list(if_clause->then_part);
		command = if_clause->else_part;
	}
	restore_fds(&fd_backup);
	return (g_last_exit_st);
}
