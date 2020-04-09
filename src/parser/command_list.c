/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 23:03:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 23:18:23 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_command	*parse_command_list(void)
{
	t_command	*first_cmd;
	t_command	*command;

	first_cmd = parse_and_or();
	command = first_cmd;
	while (g_parser.status == NOERR)
	{
		if (g_parser.token == NULL || g_parser.token->type == NEWLINE)
			break ;
		if (g_parser.token->type == AMPERSAND)
			command->flags |= CMD_AMPERSAND;
		if (g_parser.token->type == AMPERSAND || g_parser.token->type == SEMI)
		{
			token_del(&g_parser.token);
			g_parser.token = get_next_token();
		}
		command->next = parse_and_or();
		command = command->next;
	}
	if (g_parser.status != NOERR)
		command_list_del(&first_cmd);
	return (first_cmd);
}
