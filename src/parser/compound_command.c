/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compound_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** compound_command : brace_group
**                  | subshell
** 					| if_clause
*/

t_command		*parse_compound_command(void)
{
	t_command	*compound_command;

	if (!g_parser.token)
		return (NULL);
	if ((compound_command = parse_brace_group()))
		return (compound_command);
	if (g_parser.status == NOERR
	&& (compound_command = parse_subshell()))
		return (compound_command);
	if (g_parser.status == NOERR
	&& (compound_command = parse_if_clause()))
		return (compound_command);
	if (g_parser.status == NOERR
	&& (compound_command = parse_while_clause()))
		return (compound_command);
	return (NULL);
}
