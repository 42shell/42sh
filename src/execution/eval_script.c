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
** We need to evaluate the if_part in a subshell to avoid exit after evaluating
** if_part if we are already in a subshell. We keep trace of the old
** already_forked status to avoid eventual unnecessary fork when executing the
** then_part.
*/

int		eval_if_clause(t_command *command)
{
	t_if_clause	*if_clause;
	int			old_already_forked;

	old_already_forked = g_already_forked;
	if (set_redir(command->redir_list, true) != 0)
	{
		restore_fds();
		return (g_last_exit_st = 1);
	}
	if_clause = command->value.if_clause;
	while (if_clause && if_clause->if_part) //fix this shit
	{
		g_already_forked = false;
		eval_compound_list(if_clause->if_part);
		g_already_forked = old_already_forked;
		if_clause->if_part = NULL;
		if (g_last_exit_st == 0)
		{
			eval_compound_list(if_clause->then_part);
			if_clause->then_part = NULL;
			restore_fds();
			return (0);
		}
		else if (if_clause->else_part)
			if_clause = if_clause->else_part->value.if_clause;
		else
			if_clause = NULL;
	}
	if (if_clause)
	{
		eval_compound_list(if_clause->then_part);
		if_clause->then_part = NULL;
	}
	restore_fds();
	return (0);
}
