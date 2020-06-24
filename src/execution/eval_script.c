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


int		eval_if_clause(t_command *command)
{
	t_if_clause	*if_clause;

	if_clause = command->value.if_clause;
	while (if_clause && if_clause->if_part)
	{
		eval_complete_command(if_clause->if_part); //jobs problems/set pointers to NULL like subshell asyncs
		if (g_last_exit_st == 0)
		{
			eval_complete_command(if_clause->then_part);
			return (0);
		}
		else if (if_clause->else_part)
			if_clause = if_clause->else_part->value.if_clause;
		else
			if_clause = NULL;
	}
	if (if_clause)
		eval_complete_command(if_clause->then_part);
	return (0);
}
