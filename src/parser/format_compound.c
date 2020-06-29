/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_compound.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:55:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	format_compound_list(t_dstr *buf, t_command *command)
{
	while (command)
	{
		format_command(buf, command);
		ft_dstr_cat(buf, (command->sep == AMPERSAND) ? " &" : ";" );
		if (command->next)
			ft_dstr_cat(buf, " ");
		command = command->next;
	}
}

void	format_while_clause(t_dstr *buf, t_command *command)
{
	ft_dstr_cat(buf, "while ");
	format_compound_list(buf, command->value.if_clause->if_part);
	ft_dstr_cat(buf, " do " );
	format_compound_list(buf, command->value.if_clause->then_part);
	ft_dstr_cat(buf, " done" );
}

void	format_if_clause(t_dstr *buf, t_command *command)
{
	while (command)
	{
		if (command->value.if_clause->if_part)
		{
			ft_dstr_cat(buf, (command->flags & CMD_IF) ? "if " : " elif ");
			format_compound_list(buf, command->value.if_clause->if_part);
			ft_dstr_cat(buf, " then " );
		}
		else
			ft_dstr_cat(buf, " else " );
		format_compound_list(buf, command->value.if_clause->then_part);
		command = command->value.if_clause->else_part;
	}
	ft_dstr_cat(buf, " fi" );
}

void	format_group(t_dstr *buf, t_command *command)
{
	t_command	*list;

	list = command->value.compound_list;
	ft_dstr_cat(buf, (command->flags & CMD_SUBSHELL) ? "( " : "{ ");
	format_compound_list(buf, list);
	ft_dstr_cat(buf, (command->flags & CMD_SUBSHELL) ? " )" : " }");
}
