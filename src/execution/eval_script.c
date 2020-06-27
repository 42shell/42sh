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
int			eval_while_clause(t_command *command)
{
	t_if_clause	*if_clause;

	g_already_forked = false;
	if_clause = command->value.if_clause;
	if (set_redir(command->redir_list, true) != 0)
	{
		restore_fds();
		return (g_last_exit_st = 1);
	}
	while (!g_interrupt && eval_compound_list(if_clause->if_part) == 0)
	{
		eval_compound_list(if_clause->then_part); //if 140 break
		if (g_last_exit_st == 130 || g_interrupt)
			break ;
	}
	restore_fds();
	return (0);
}
*/

static int	eval_if_statement(t_if_clause *if_clause)
{
	int		old_already_forked;

	old_already_forked = g_already_forked;
	g_already_forked = false;
	eval_compound_list(if_clause->if_part);
	g_already_forked = old_already_forked;
	//if_clause->if_part = NULL;
	if (g_last_exit_st == 0)
	{
		eval_compound_list(if_clause->then_part);
		//if_clause->then_part = NULL;
		return (1);
	}
	return (0);
}

/*
** We need to evaluate the if_part in a subshell to avoid exit after evaluating
** if_part if we are already in a subshell. We keep trace of the old
** already_forked status to avoid eventual unnecessary fork when executing the
** then_part.
*/

int			eval_if_clause(t_command *command)
{
	t_if_clause	*if_clause;

	if (set_redir(command->redir_list, true) != 0)
	{
		restore_fds();
		return (g_last_exit_st = 1);
	}
	while (command)
	{
		if_clause = command->value.if_clause;
		if (if_clause->if_part)
		{
			if (eval_if_statement(if_clause) == 1)
				break ;
		}
		else
		{
			eval_compound_list(if_clause->then_part);
			//if_clause->then_part = NULL;
		}
		command = if_clause->else_part;
	}
	restore_fds();
	return (0);
}
