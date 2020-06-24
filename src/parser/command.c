/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** command         : simple_command
**                 | compound_command
**                 | compound_command redirect_list
*/

/*
** need fucking t_compound_command
*/

t_command		*parse_command(void)
{
	t_command	*command;

	command = NULL;
	if ((command = parse_simple_command()))
		return (command);
	if (g_parser.status == NOERR
	&& (command = parse_compound_command()))
	{
		if (command->type != IF_CLAUSE)
			command->value.group->redir_list = parse_redirect_list();
		if (g_parser.status != NOERR)
		{
			complete_command_del(&command);
			return (NULL);
		}
		return (command);
	}
	return (NULL);
}
