/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:34:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_command	*return_parse_error(t_command **to_del)
{
	if (!g_parser.status)
		g_parser.status = UNEXPECTED_TOKEN;
	complete_command_del(to_del);
	return (NULL);
}

int			parse_error(char *near)
{
	g_last_exit_st = 2;
	if (g_parser.status != USER_ABORT)
	{
		if (g_parser.status == BRACKET_NEST_LIMIT_REACHED)
			ft_dprintf(2,
			"42sh: too many nested brackets, aborting\n");
		else
			ft_dprintf(2,
			"42sh: syntax error near unexpected token '%s'\n",
			near);
	}
	g_parser.status = NOERR;
	return (0);
}
